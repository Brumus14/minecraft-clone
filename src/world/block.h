#ifndef BLOCK_H
#define BLOCK_H

#include "../graphics/bo.h"
#include "../graphics/vao.h"
#include "../math/vector3.h"
#include "../graphics/texture.h"
#include "../tilemap.h"

typedef enum block_type { BLOCK_TYPE_GRASS } block_type;

typedef struct block_texture {
    int face_textures[6]; // front, top, right, bottom, left, back
} block_texture;

typedef struct block {
    bo vbo;
    bo ibo;
    vao vao;
    tilemap *tilemap;
} block;

void block_init(block *block, vector3 position, block_type type,
                tilemap *tilemap);
void block_draw(block *block);

#endif
