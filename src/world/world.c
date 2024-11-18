#include "world.h"

#include <string.h>

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
}

block_type generation_algorithm(vector3i position) {
    if (position.x < 1 || position.z < 3) {
        return BLOCK_TYPE_STONE;
    }

    return BLOCK_TYPE_GRASS;
}

void world_load_chunk(world *world, vector3i position) {
    if (get_chunk_index(world, position) != -1) {
        return;
    }

    world->chunk_count++;

    world->chunks = realloc(world->chunks, sizeof(chunk) * world->chunk_count);

    chunk *chunk = &world->chunks[world->chunk_count - 1];

    chunk_init(chunk, position, world->tilemap);

    for (int z = 0; z < CHUNK_SIZE_Z; z++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int x = 0; x < CHUNK_SIZE_X; x++) {

                vector3i global_position;
                vector3i_init(&global_position, position.x * CHUNK_SIZE_X + x,
                              position.y * CHUNK_SIZE_Y + y,
                              position.z * CHUNK_SIZE_Z + z);

                chunk->blocks[z][y][x].type = generation_algorithm((vector3i){
                    global_position.x, global_position.y, global_position.z});
            }
        }
    }

    chunk_calculate_active_faces(chunk); // MAYBE DO THIS AUTOMATICALLY
    chunk_generate_vertices_indices(chunk);
}

void world_unload_chunk(world *world, vector3i position) {
    int chunk_index = get_chunk_index(world, position);
    /*printf("unload!\n");*/
    /*printf("UNLOADING: %d, %d, %d\n", position.x, position.y, position.z);*/

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
    for (int i = 0; i < world->chunk_count; i++) {
        chunk_draw(&world->chunks[i]);
    }
}
