#include "world.h"

#include <string.h>
#include "noise1234.h"
#include "../math/math_util.h"

int get_chunk_index(world *world, vector3i position) {
    for (int i = 0; i < world->chunk_count; i++) {
        if (vector3i_equal(world->chunks[i].position, position)) {
            return i;
        }
    }

    return -1;
}

void world_init(world *world, tilemap *tilemap) {
    world->tilemap = tilemap;
    world->chunk_count = 0;
    world->chunks = malloc(0);
    world->chunks_to_generate_count = 0;
    world->chunks_to_generate = malloc(0);
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

                    if (position.y <
                        (noise3(position.x * 0.03, position.z * 0.03, 1.0)) *
                            3 * 16) {
                        type = BLOCK_TYPE_DIRT;
                    }

                    chunk->blocks[z][y][x] = type;
                }
            }
        }
    }

    if (stage == 1) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            for (int y = 0; y < CHUNK_SIZE_Y; y++) {
                for (int x = 0; x < CHUNK_SIZE_X; x++) {
                    vector3i position = {chunk->position.x * CHUNK_SIZE_X + x,
                                         chunk->position.y * CHUNK_SIZE_Y + y,
                                         chunk->position.z * CHUNK_SIZE_Z + z};

                    if (chunk->blocks[z][y][x] == BLOCK_TYPE_DIRT &&
                        world_get_block(world,
                                        (vector3i){position.x, position.y + 1,
                                                   position.z}) ==
                            BLOCK_TYPE_EMPTY) {
                        chunk->blocks[z][y][x] = BLOCK_TYPE_GRASS;
                    }
                }
            }
        }
    }
}

void world_load_chunk(world *world, vector3i position) {
    if (get_chunk_index(world, position) != -1) {
        return;
    }

    world->chunk_count++;

    world->chunks = realloc(world->chunks, sizeof(chunk) * world->chunk_count);

    chunk *new_chunk = &world->chunks[world->chunk_count - 1];

    chunk_init(new_chunk, position, world->tilemap);

    world->chunks_to_generate_count++;

    world->chunks_to_generate =
        realloc(world->chunks_to_generate,
                sizeof(chunk *) * world->chunks_to_generate_count);
    world->chunks_to_generate[world->chunks_to_generate_count - 1] =
        world->chunk_count - 1;
}

void generate_chunks(world *world) {
    for (int i = 0; i < world->chunks_to_generate_count; i++) {
        chunk *chunk = &world->chunks[world->chunks_to_generate[i]];

        generate_chunk_terrain(world, chunk, 0);
    }

    for (int i = 0; i < world->chunks_to_generate_count; i++) {
        chunk *chunk = &world->chunks[world->chunks_to_generate[i]];

        generate_chunk_terrain(world, chunk, 1);
    }

    for (int i = 0; i < world->chunks_to_generate_count; i++) {
        chunk *chunk = &world->chunks[world->chunks_to_generate[i]];

        chunk_update(chunk);
    }

    world->chunks_to_generate_count = 0;
    world->chunks_to_generate = malloc(0);
}

void world_unload_chunk(world *world, vector3i position) {
    int chunk_index = get_chunk_index(world, position);

    if (chunk_index == -1) {
        return;
    }

    world->chunk_count--;

    if (chunk_index < world->chunk_count) {
        memcpy(&world->chunks[chunk_index], &world->chunks[world->chunk_count],
               sizeof(chunk));
    }

    world->chunks = realloc(world->chunks, sizeof(chunk) * world->chunk_count);
}

void world_draw(world *world) {
    generate_chunks(world);

    for (int i = 0; i < world->chunk_count; i++) {
        chunk_draw(&world->chunks[i]);
    }
}

// use mipmapping
block_type world_get_block(world *world, vector3i position) {
    // rename to chunks loaded
    vector3i chunk_position = {floorf((float)position.x / CHUNK_SIZE_X),
                               floorf((float)position.y / CHUNK_SIZE_Y),
                               floorf((float)position.z / CHUNK_SIZE_Z)};

    int chunk_index = get_chunk_index(world, chunk_position);

    if (chunk_index == -1) {
        return BLOCK_TYPE_EMPTY;
    }

    chunk *chunk = &world->chunks[chunk_index];

    vector3i block_chunk_position = {mod(position.x, CHUNK_SIZE_X),
                                     mod(position.y, CHUNK_SIZE_Y),
                                     mod(position.z, CHUNK_SIZE_Z)};

    return chunk->blocks[block_chunk_position.z][block_chunk_position.y]
                        [block_chunk_position.x];
}
