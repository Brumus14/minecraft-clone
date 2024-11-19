#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
#include "../math/vector3.h"
#include "../graphics/graphics.h"
#include "../tilemap.h"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_Z 16

typedef struct chunk {
    vector3i position;
    block_type blocks[CHUNK_SIZE_Z][CHUNK_SIZE_Y][CHUNK_SIZE_X];
    tilemap *tilemap;
    bo vbo;
    bo ibo;
    vao vao;
} chunk;

void chunk_init(chunk *chunk, vector3i position, tilemap *tilemap);
void chunk_draw(chunk *chunk);

#endif
