#ifndef WORLD_H
#define WORLD_H

#include <pthread.h>
#include "chunk.h"
#include "../linked_list.h"
#include "../queue.h"

typedef struct world {
    tilemap tilemap;
    int chunks_count;
    linked_list chunks;
    queue chunks_to_generate;
    int chunks_to_generate_count;
    float seed;
    pthread_t generation_thread;
} world;

void world_init(world *world);
void world_load_chunk(world *world, vector3i position);
void world_unload_chunk(world *world, vector3i position);
void world_draw(world *world);
block_type world_get_block(world *world, vector3d position);
void world_set_block(world *world, block_type type, vector3d position);

#endif
