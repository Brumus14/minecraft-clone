#include "world.h"

#include <string.h>
#include <pthread.h>
#include "noise1234.h"
#include "../math/math_util.h"

int get_chunk_index(world *world, vector3i position) {
    for (int i = 0; i < world->chunks_count; i++) {
        chunk *chunk = linked_list_get(&world->chunks, i);

        if (vector3i_equal(chunk->position, position)) {
            return i;
        }
    }

    return -1;
}

void generate_chunk_terrain(world *world, chunk *chunk, int stage) {
    if (stage == 0) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            for (int y = 0; y < CHUNK_SIZE_Y; y++) {
                for (int x = 0; x < CHUNK_SIZE_X; x++) {
                    vector3i position;
                    vector3i_init(&position,
                                  chunk->position.x * CHUNK_SIZE_X + x,
                                  chunk->position.y * CHUNK_SIZE_Y + y,
                                  chunk->position.z * CHUNK_SIZE_Z + z);

                    block_type type = BLOCK_TYPE_EMPTY;

                    if (position.y < (noise3(position.x * 0.03,
                                             position.z * 0.03, world->seed)) *
                                             16 -
                                         8) {
                        type = BLOCK_TYPE_STONE;
                    } else if (position.y <
                               (noise3(position.x * 0.03, position.z * 0.03,
                                       world->seed)) *
                                   16) {
                        type = BLOCK_TYPE_DIRT;
                    }

                    chunk->blocks[z][y][x] = type;
                }
            }
        }
    }

    // better ore generation
    // use octaves and make noise generator abstraction
    if (stage == 1) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            for (int y = 0; y < CHUNK_SIZE_Y; y++) {
                for (int x = 0; x < CHUNK_SIZE_X; x++) {
                    vector3i position = {chunk->position.x * CHUNK_SIZE_X + x,
                                         chunk->position.y * CHUNK_SIZE_Y + y,
                                         chunk->position.z * CHUNK_SIZE_Z + z};

                    if (chunk->blocks[z][y][x] == BLOCK_TYPE_DIRT &&
                        world_get_block(world,
                                        (vector3d){position.x, position.y + 1,
                                                   position.z}) ==
                            BLOCK_TYPE_EMPTY) {
                        chunk->blocks[z][y][x] = BLOCK_TYPE_GRASS;
                    } else if (chunk->blocks[z][y][x] == BLOCK_TYPE_STONE) {
                        if (noise4(position.x * 0.2, position.y * 0.2,
                                   position.z * 0.2, world->seed) > 0.6) {
                            chunk->blocks[z][y][x] = BLOCK_TYPE_COAL;
                        } else if (noise4(position.x * 0.2, position.y * 0.2,
                                          position.z * 0.2,
                                          world->seed * 1.419198) > 0.6) {
                            chunk->blocks[z][y][x] = BLOCK_TYPE_DIAMOND;
                        }
                    }
                }
            }
        }
    }
}

// void *generation_thread_main(void *world_arg) {
//     world *world_pointer = (world *)world_arg;
//
//     // copy the data so dont have to keep chunks to generate locked for main
//     // thread
//     while (true) {
//     }
//
//     return NULL;
// }

void world_init(world *world) {
    tilemap_init(&world->tilemap, "res/textures/atlas.png",
                 TEXTURE_FILTER_NEAREST, 16, 16, 1, 2);

    linked_list_init(&world->chunks);
    world->chunks_to_generate_count = 0;
    world->chunks_to_generate = NULL;

    world->seed = random_range(0, 100);
}

void world_load_chunk(world *world, vector3i position) {
    if (get_chunk_index(world, position) != -1) {
        return;
    }

    chunk *new_chunk = malloc(sizeof(chunk));
    linked_list_insert_end(&world->chunks, new_chunk);
    world->chunks_count++;

    chunk_init(new_chunk, position, &world->tilemap);
    new_chunk->visible = false;

    world->chunks_to_generate_count++;

    world->chunks_to_generate =
        realloc(world->chunks_to_generate,
                sizeof(chunk *) * world->chunks_to_generate_count);
    world->chunks_to_generate[world->chunks_to_generate_count - 1] =
        world->chunks_count - 1;
}

// remove chunks from chunks to generate if have moved out of render distance
// before generated
void generate_chunks(world *world) {
    if (world->chunks_to_generate_count == 0) {
        return;
    }

    chunk *chunk = linked_list_get(
        &world->chunks,
        world->chunks_to_generate[world->chunks_to_generate_count - 1]);
    generate_chunk_terrain(world, chunk, 0);
    generate_chunk_terrain(world, chunk, 1);
    chunk_update(chunk);
    chunk->visible = true;

    world->chunks_to_generate_count--;
    world->chunks_to_generate =
        realloc(world->chunks_to_generate,
                sizeof(int) * world->chunks_to_generate_count);

    // for (int i = 0; i < world->chunks_to_generate_count; i++) {
    //     chunk *chunk =
    //         linked_list_get(&world->chunks, world->chunks_to_generate[i]);
    //
    //     generate_chunk_terrain(world, chunk, 0);
    // }
    //
    // for (int i = 0; i < world->chunks_to_generate_count; i++) {
    //     chunk *chunk = &world->chunks[world->chunks_to_generate[i]];
    //
    //     generate_chunk_terrain(world, chunk, 1);
    //     chunk_update(chunk);
    // }
    //
    // world->chunks_to_generate_count = 0;
    // world->chunks_to_generate = malloc(0);
}

void world_unload_chunk(world *world, vector3i position) {
    int chunk_index = get_chunk_index(world, position);

    if (chunk_index == -1) {
        return;
    }

    linked_list_remove(&world->chunks, chunk_index);
    world->chunks_count--;
}

void world_draw(world *world) {
    generate_chunks(world);

    texture_bind(&world->tilemap.texture);

    for (int i = 0; i < world->chunks_count; i++) {
        chunk_draw(linked_list_get(&world->chunks, i));
    }
}

// use mipmapping
block_type world_get_block(world *world, vector3d position) {
    // rename to chunks loaded
    vector3i chunk_position = {floor(position.x / CHUNK_SIZE_X),
                               floor(position.y / CHUNK_SIZE_Y),
                               floor(position.z / CHUNK_SIZE_Z)};

    int chunk_index = get_chunk_index(world, chunk_position);

    if (chunk_index == -1) {
        return BLOCK_TYPE_EMPTY;
    }

    chunk *chunk = linked_list_get(&world->chunks, chunk_index);

    vector3i block_chunk_position = {mod(floor(position.x), CHUNK_SIZE_X),
                                     mod(floor(position.y), CHUNK_SIZE_Y),
                                     mod(floor(position.z), CHUNK_SIZE_Z)};

    return chunk->blocks[block_chunk_position.z][block_chunk_position.y]
                        [block_chunk_position.x];
}

void world_set_block(world *world, block_type type, vector3d position) {
    vector3i chunk_position = {floor(position.x / CHUNK_SIZE_X),
                               floor(position.y / CHUNK_SIZE_Y),
                               floor(position.z / CHUNK_SIZE_Z)};

    int chunk_index = get_chunk_index(world, chunk_position);

    if (chunk_index == -1) {
        return;
    }

    chunk *chunk = linked_list_get(&world->chunks, chunk_index);

    vector3i block_chunk_position = {mod(floor(position.x), CHUNK_SIZE_X),
                                     mod(floor(position.y), CHUNK_SIZE_Y),
                                     mod(floor(position.z), CHUNK_SIZE_Z)};

    chunk->blocks[block_chunk_position.z][block_chunk_position.y]
                 [block_chunk_position.x] = type;

    chunk_update(chunk);
}
