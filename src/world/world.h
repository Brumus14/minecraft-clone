#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"
#include "../data_structures/linked_list.h"
#include "../thread_pool.h"

#define WORLD_WORKER_COUNT 4

typedef struct world {
    // Use hashmap
    linked_list chunks; // Should this have a mutex?
    tilemap tilemap;
    float seed;
    thread_pool workers;
} world;

void world_init(world *world);
void world_load_chunk(world *world, vector3i position);
void world_unload_chunk(world *world, vector3i position);
void world_draw(world *world);
block_type world_get_block(world *world, vector3d position);
void world_set_block(world *world, block_type type, vector3d position);

#endif
