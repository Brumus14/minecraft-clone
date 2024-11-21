#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"

typedef struct world {
    tilemap *tilemap;
    int chunk_count;
    chunk *chunks;
    int *chunks_to_generate;
    int chunks_to_generate_count;
} world;

void world_init(world *world, tilemap *tilemap);
void world_load_chunk(world *world, vector3i position);
void world_unload_chunk(world *world, vector3i position);
void world_draw(world *world);
block_type world_get_block(world *world, vector3i position);

#endif
