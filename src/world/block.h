#ifndef BLOCK_H
#define BLOCK_H

#include "../graphics/bo.h"
#include "../graphics/vao.h"
#include "../math/vector3.h"
#include "../graphics/texture.h"
#include "../tilemap.h"

static bool ACTIVE_FACES_ALL[6] = {true, true, true, true, true, true};

typedef enum block_type {
    BLOCK_TYPE_EMPTY,
    BLOCK_TYPE_GRASS,
    BLOCK_TYPE_DIRT
} block_type;

typedef struct block_texture {
    bool empty;
    // dont need to store this in every block
    int face_textures[6]; // front, top, right, bottom, left, back
} block_texture;

typedef struct block {
    vector3 position;
    block_type type;
    bo vbo;
    bo ibo;
    vao vao;
    tilemap *tilemap;
    bool active_faces[6];
    int active_face_count;
} block;

void block_init(block *block, vector3 position, block_type type,
                bool *active_faces, tilemap *tilemap);
void block_draw(block *block);

#endif
