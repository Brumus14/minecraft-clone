#ifndef BLOCK_H
#define BLOCK_H

typedef enum block_type {
    BLOCK_TYPE_EMPTY,
    BLOCK_TYPE_GRASS,
    BLOCK_TYPE_DIRT,
    BLOCK_TYPE_STONE,
    BLOCK_TYPE_COAL,
    BLOCK_TYPE_LOG,
    BLOCK_TYPE_DIAMOND,
} block_type;

typedef enum block_face {
    BLOCK_FACE_FRONT,
    BLOCK_FACE_TOP,
    BLOCK_FACE_RIGHT,
    BLOCK_FACE_BOTTOM,
    BLOCK_FACE_LEFT,
    BLOCK_FACE_BACK,
} block_face;

typedef struct block {
    block_type type;
} block;

// rename to texture indexes?
typedef struct block_texture {
    int face_texture_indices[6]; // front, top, right, bottom, left, back
} block_texture;

block_texture block_type_to_texture(block_type type);

#endif
