#include "world.h"

void world_init(world *world, vector3 size, tilemap *tilemap) {
    world->size = size;
    world->tilemap = tilemap;

    world->chunks = malloc(sizeof(chunk) * size.x * size.y * size.z);

    int i = 0;

    for (int z = 0; z < size.z; z++) {
        for (int y = 0; y < size.y; y++) {
            for (int x = 0; x < size.x; x++) {
                chunk_init(&world->chunks[i],
                           vector3_scalar_multiply((vector3){x, y, z}, 16),
                           tilemap);

                chunk_calculate_active_faces(&world->chunks[i]);
                chunk_generate_vertices_indices(&world->chunks[i]);

                i++;
            }
        }
    }
}

void world_draw(world *world) {
    int i = 0;

    for (int z = 0; z < world->size.z; z++) {
        for (int y = 0; y < world->size.y; y++) {
            for (int x = 0; x < world->size.x; x++) {
                chunk_draw(&world->chunks[i]);
                i++;
            }
        }
    }
}
