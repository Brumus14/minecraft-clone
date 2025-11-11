#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
#include "../math/vector3.h"
#include "../graphics/graphics.h"
#include "../tilemap.h"
#include <pthread.h>

#define CHUNK_SIZE_X 32
#define CHUNK_SIZE_Y 32
#define CHUNK_SIZE_Z 32

#define CHUNK_VERTEX_SIZE 8

typedef enum chunk_state {
    CHUNK_STATE_NEEDS_TERRAIN,
    CHUNK_STATE_GENERATING_TERRAIN,
    CHUNK_STATE_NEEDS_MESH,
    CHUNK_STATE_GENERATING_MESH,
    CHUNK_STATE_NEEDS_BUFFERS,
    CHUNK_STATE_READY,
} chunk_state;

typedef struct chunk {
    bool visible;
    chunk_state state;
    vector3i position;
    block_type blocks[CHUNK_SIZE_Z][CHUNK_SIZE_Y][CHUNK_SIZE_X];
    tilemap *tilemap;
    float *vertices;
    unsigned int *indices;
    int face_count;
    bo vbo;
    bo ibo;
    vao vao;
    pthread_mutex_t mutex; // REMOVE?
} chunk;

// What even are these
// clang-format off
static const float VERTEX_POSITIONS[8][3] = {
    {0, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 1, 1},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
    {0, 1, 0},
};

static const unsigned int FACE_INDICES[6][4] = {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {1, 5, 6, 2},
    {4, 5, 1, 0},
    {4, 0, 3, 7},
    {5, 4, 7, 6},
};

static const float FACE_NORMALS[6][3] = {
    {0,  0,  1 },
    {0,  1,  0 },
    {1,  0,  0 },
    {0,  -1, 0 },
    {-1, 0,  0 },
    {0,  0,  -1},
};

static const int INDEX_ORDER[6] = {0, 1, 2, 0, 2, 3};

void chunk_init(chunk *chunk, vector3i position, tilemap *tilemap);
void chunk_destroy(chunk *chunk);
void chunk_update_mesh(chunk *chunk);
void chunk_update_buffers(chunk *chunk);
void chunk_draw(chunk *chunk);

#endif
