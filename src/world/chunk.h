#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_Z 16

typedef struct chunk {
    vector3i position;
    block blocks[CHUNK_SIZE_Z][CHUNK_SIZE_Y][CHUNK_SIZE_X];
    tilemap *tilemap;
    bo vbo;
    bo ibo;
    vao vao;
} chunk;

void chunk_init(chunk *chunk, vector3i position, tilemap *tilemap);
void chunk_calculate_active_faces(chunk *chunk);
void chunk_generate_vertices_indices(chunk *chunk);
void chunk_draw(chunk *chunk);

void greedy_mesh_vertices_indices();

#endif
