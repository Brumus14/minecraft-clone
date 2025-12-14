#ifndef WORKER_H
#define WORKER_H

#include "chunk.h"
#include "world.h"

#include "../util/thread_pool.h"
#include "../data_structures/hash_map.h"

typedef struct worker_generate_chunk_terrain_args {
    chunk *chunk;
    float seed;
} worker_generate_chunk_terrain_args;

typedef struct worker_generate_chunk_mesh_args {
    chunk *chunk;
    world *world;
    // hash_map *chunks;
} worker_generate_chunk_mesh_args;

typedef struct worker_generate_chunk_args {
    chunk *chunk;
    float seed;
    thread_pool *workers;
} worker_generate_chunk_args;

void *worker_generate_chunk_terrain(void *arg);
void *worker_generate_chunk_mesh(void *arg);
void *worker_generate_chunk(void *arg);

#endif
