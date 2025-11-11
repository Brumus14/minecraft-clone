#include "worker.h"
#include "block.h"
#include "chunk.h"
#include "world_generation.h"
#include <stdio.h>
#include <string.h>

void *worker_generate_chunk_terrain(void *arg) {
    worker_generate_chunk_terrain_args *args = arg;
    chunk *chunk = args->chunk;
    float seed = args->seed;

    chunk->state = CHUNK_STATE_GENERATING_TERRAIN;

    block_type *terrain = world_generation_chunk_terrain(chunk->position, seed);
    memcpy(chunk->blocks, terrain, sizeof(chunk->blocks));
    free(terrain);

    chunk->state = CHUNK_STATE_NEEDS_MESH;

    printf("generated terrain for %d,%d,%d\n", chunk->position.x,
           chunk->position.y, chunk->position.z);

    return NULL;
}

void *worker_generate_chunk_mesh(void *arg) {
    chunk *chunk = arg;

    chunk->state = CHUNK_STATE_GENERATING_MESH;
    chunk_update_mesh(chunk);
    chunk->state = CHUNK_STATE_NEEDS_BUFFERS;

    printf("generated mesh for %d,%d,%d\n", chunk->position.x,
           chunk->position.y, chunk->position.z);

    return NULL;
}

void *worker_generate_chunk(void *arg) {
    worker_generate_chunk_args *args = arg;
    chunk *chunk = args->chunk;
    float seed = args->seed;

    worker_generate_chunk_terrain(arg);
    worker_generate_chunk_mesh(chunk);
    chunk->visible = true;

    return NULL;
}
