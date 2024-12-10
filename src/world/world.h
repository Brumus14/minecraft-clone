#ifndef WORLD_H
#define WORLD_H

#include <pthread.h>
#include "chunk.h"
#include "../data_structures/safe_linked_list.h"
#include "../data_structures/safe_queue.h"
#include "worker.h"

typedef struct world {
    safe_linked_list chunks;
    worker worker;
    tilemap tilemap;
    float seed;
    pthread_t worker_thread;
} world;

void world_init(world *world);
void world_load_chunk(world *world, vector3i position);
void world_unload_chunk(world *world, vector3i position);
void world_draw(world *world);
block_type world_get_block(world *world, vector3d position);
void world_set_block(world *world, block_type type, vector3d position);

void world_generate_chunk_terrain(world *world, chunk *chunk, int stage);

#endif
