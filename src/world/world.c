#include "world.h"

#include <string.h>
#include "noise1234.h"

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

block_type terrain_generation_algorithm(vector3i position) {
    block_type type = BLOCK_TYPE_EMPTY;

    if (position.y <
        (noise3(position.x * 0.03, position.z * 0.03, 1.0) - 1) * -16) {
        type = BLOCK_TYPE_GRASS;
    }

    return type;
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

                chunk->blocks[z][y][x] = terrain_generation_algorithm(
                    (vector3i){global_position.x, global_position.y,
                               global_position.z});
            }
        }
    }

    chunk_update(chunk);
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
    for (int i = 0; i < world->chunk_count; i++) {
        chunk_draw(&world->chunks[i]);
    }
}
