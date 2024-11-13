#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"

typedef struct world {
    vector3 size;
    tilemap *tilemap;
    chunk *chunks;
} world;

void world_init(world *world, vector3 size, tilemap *tilemap);
void world_draw(world *world);

#endif
