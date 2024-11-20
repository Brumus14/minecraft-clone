#include "block.h"

block_texture block_type_to_texture(block_type type) {
    // front, top, right, bottom, left, back
    switch (type) {
    case BLOCK_TYPE_EMPTY:
        return (block_texture){0, 0, 0, 0, 0};
    case BLOCK_TYPE_GRASS:
        return (block_texture){
            .face_texture_indices = {1, 0, 1, 2, 1, 1}
        };
    case BLOCK_TYPE_DIRT:
        return (block_texture){
            .face_texture_indices = {3, 2, 3, 2, 3, 3}
        };
    case BLOCK_TYPE_STONE:
        return (block_texture){
            .face_texture_indices = {4, 4, 4, 4, 4, 4}
        };
    }
}
