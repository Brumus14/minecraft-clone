#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"

typedef struct chunk {
    block blocks[16][16][16];
    tilemap *tilemap;
} chunk;

void chunk_init(chunk *chunk, tilemap *tilemap);
void chunk_calculate_active_faces(chunk *chunk);
void chunk_draw(chunk *chunk);

#endif
