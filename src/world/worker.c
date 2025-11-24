#include "worker.h"
#include "block.h"
#include "chunk.h"
#include "world_generation.h"
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>

// TODO: Check if the chunk has been unloaded if so then cancel
void *worker_generate_chunk_terrain(void *arg) {
    worker_generate_chunk_terrain_args *args = arg;
    chunk *chunk = args->chunk;
    float seed = args->seed;

    atomic_fetch_add(&chunk->in_use, 1);
    atomic_store(&chunk->state, CHUNK_STATE_GENERATING_TERRAIN);

    block_type *terrain = world_generation_chunk_terrain(chunk->position, seed);

    pthread_mutex_lock(&chunk->lock);
    memcpy(chunk->blocks, terrain, sizeof(chunk->blocks));

    printf("generated terrain for %d,%d,%d\n", chunk->position.x,
           chunk->position.y, chunk->position.z);
    pthread_mutex_unlock(&chunk->lock);

    free(terrain);

    atomic_store(&chunk->state, CHUNK_STATE_NEEDS_MESH);
    atomic_fetch_sub(&chunk->in_use, 1);

    return NULL;
}

void *worker_generate_chunk_mesh(void *arg) {
    chunk *chunk = arg;

    atomic_fetch_add(&chunk->in_use, 1);
    atomic_store(&chunk->state, CHUNK_STATE_GENERATING_MESH);

    pthread_mutex_lock(&chunk->lock);
    chunk_update_mesh(chunk);

    printf("generated mesh for %d,%d,%d\n", chunk->position.x,
           chunk->position.y, chunk->position.z);
    pthread_mutex_unlock(&chunk->lock);

    atomic_store(&chunk->state, CHUNK_STATE_NEEDS_BUFFERS);
    atomic_fetch_sub(&chunk->in_use, 1);

    return NULL;
}

void *worker_generate_chunk(void *arg) {
    worker_generate_chunk_args *args = arg;
    chunk *chunk = args->chunk;
    float seed = args->seed;

    atomic_fetch_add(&chunk->in_use, 1);

    worker_generate_chunk_terrain(arg);
    // worker_generate_chunk_mesh(chunk);
    atomic_store(&chunk->visible, true);

    atomic_fetch_sub(&chunk->in_use, 1);

    return NULL;
}
