#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"
#include "../data_structures/linked_list.h"
#include "../data_structures/hash_map.h"
#include "../util/thread_pool.h"

#define WORLD_WORKER_COUNT 12
#define CHUNKS_BUCKET_COUNT 1024

typedef struct world {
    hash_map chunks;
    pthread_rwlock_t chunks_lock; // Is this needed
    tilemap tilemap;
    float seed;
    thread_pool workers;
} world;

void world_init(world *world);
void world_load_chunk(world *world, vector3i position);
void world_unload_chunk(world *world, vector3i position);
void world_draw(world *world);
block_type world_get_block(world *world, vector3i position);
block_type world_get_block_safe(world *world, vector3i position);
void world_set_block(world *world, block_type type, vector3i position);
void world_set_block_safe(world *world, block_type type, vector3i position);

#endif
