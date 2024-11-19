#ifndef BLOCK_H
#define BLOCK_H

typedef enum block_type {
    BLOCK_TYPE_EMPTY,
    BLOCK_TYPE_GRASS,
    BLOCK_TYPE_DIRT,
    BLOCK_TYPE_STONE
} block_type;

typedef struct block {
    block_type type;
} block;

#endif
