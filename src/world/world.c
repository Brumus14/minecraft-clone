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

void world_load_chunk(world *world, vector3i position) {
    if (get_chunk_index(world, position) != -1) {
        return;
    }

    world->chunk_count++;

    world->chunks = realloc(world->chunks, sizeof(chunk) * world->chunk_count);

    chunk_init(&world->chunks[world->chunk_count - 1], position,
               world->tilemap);
    chunk_calculate_active_faces(&world->chunks[world->chunk_count - 1]);
    chunk_generate_vertices_indices(&world->chunks[world->chunk_count - 1]);
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
