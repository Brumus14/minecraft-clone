#ifndef WORKER_H
#define WORKER_H

#include "chunk.h"

typedef struct worker_generate_chunk_terrain_args {
    chunk *chunk;
    float seed;
} worker_generate_chunk_terrain_args;

typedef struct worker_generate_chunk_args {
    chunk *chunk;
    float seed;
} worker_generate_chunk_args;

void *worker_generate_chunk_terrain(void *arg);
void *worker_generate_chunk_mesh(void *arg);
void *worker_generate_chunk(void *arg);

#endif
