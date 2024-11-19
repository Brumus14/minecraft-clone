#ifndef BLOCK_H
#define BLOCK_H

#include "../graphics/bo.h"
#include "../graphics/vao.h"
#include "../math/vector3.h"
#include "../graphics/texture.h"
#include "../tilemap.h"

static bool ACTIVE_FACES_ALL[6] = {true, true, true, true, true, true};
static bool ACTIVE_FACES_NONE[6] = {false, false, false, false, false, false};

typedef enum block_type {
    BLOCK_TYPE_EMPTY,
    BLOCK_TYPE_GRASS,
    BLOCK_TYPE_DIRT,
    BLOCK_TYPE_STONE
} block_type;

typedef struct block_texture {
    bool empty;
    // dont need to store this in every block
    int face_textures[6]; // front, top, right, bottom, left, back
} block_texture;

typedef struct block { // DEBLOAT THIS
    vector3i position;
    block_type type;
    float *vertices;
    int vertex_count;
    unsigned int *indices;
    int index_count;
    tilemap *tilemap;
    bool active_faces[6];
    int active_face_count;
} block;

void block_init(block *block, vector3i position, block_type type,
                bool *active_faces, tilemap *tilemap);
void block_term(block *block);

#endif
