#ifndef ITEMS_H
#define ITEMS_H

#include "world/block.h"

typedef enum item_type {
    ITEM_TYPE_EMPTY,
    ITEM_TYPE_GRASS_BLOCK,
    ITEM_TYPE_DIRT_BLOCK,
    ITEM_TYPE_STONE_BLOCK,
    ITEM_TYPE_COAL_BLOCK,
    ITEM_TYPE_LOG_BLOCK,
    ITEM_TYPE_DIAMOND_BLOCK,
} item_type;

block_type item_type_to_block_type(item_type type);
item_type block_type_to_item_type(block_type type);
int item_type_to_texture_index(item_type type);

#endif
