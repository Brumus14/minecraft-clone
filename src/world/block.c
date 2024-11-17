#include "block.h"

#include <string.h>

block_texture block_type_to_texture(block_type type) {
    // front, top, right, bottom, left, back
    switch (type) {
    case BLOCK_TYPE_EMPTY:
        return (block_texture){.empty = true};
    case BLOCK_TYPE_GRASS:
        return (block_texture){
            .empty = false, .face_textures = {1, 0, 1, 2, 1, 1}
        };
    case BLOCK_TYPE_DIRT:
        return (block_texture){
            .empty = false, .face_textures = {3, 2, 3, 2, 3, 3}
        };
    case BLOCK_TYPE_STONE:
        return (block_texture){
            .empty = false, .face_textures = {4, 4, 4, 4, 4, 4}
        };
    }
}

void generate_vertices(block *block, float *vertices, unsigned int *indices) {
    block_texture texture = block_type_to_texture(block->type);

    float positions[][3] = {
        {0.0, -1.0, 1.0},
        {1.0, -1.0, 1.0},
        {1.0, 0.0,  1.0},
        {0.0, 0.0,  1.0},
        {0.0, -1.0, 0.0},
        {1.0, -1.0, 0.0},
        {1.0, 0.0,  0.0},
        {0.0, 0.0,  0.0},
    };

    int face_vertices[][4] = {
        {0, 1, 2, 3},
        {3, 2, 6, 7},
        {1, 5, 6, 2},
        {4, 5, 1, 0},
        {4, 0, 3, 7},
        {5, 4, 7, 6},
    };

    memcpy(indices,
           (unsigned int[]){
               0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,
               8,  9,  10, 8,  10, 11, 12, 13, 14, 12, 14, 15,
               16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23,
           },
           sizeof(int) * 6 * block->active_face_count);

    int faces_added = 0;

    for (int i = 0; i < 6; i++) {
        if (block->active_faces[i] == false) {
            continue;
        }

        rectangle rect = tilemap_get_tile_rectangle(block->tilemap,
                                                    texture.face_textures[i]);

        for (int j = 0; j < 4; j++) {
            // just set the vertices array no vertex array
            float vertex[5];

            memcpy(vertex, positions[face_vertices[i][j]], sizeof(float) * 3);

            vertex[0] += block->position.x;
            vertex[1] += block->position.y;
            vertex[2] += block->position.z;

            vertex[3] = rect.x;
            vertex[4] = rect.y;

            switch (j) {
            case 0:
                vertex[4] += rect.height;
                break;
            case 1:
                vertex[3] += rect.width;
                vertex[4] += rect.height;
                break;
            case 2:
                vertex[3] += rect.width;
                break;
            }

            memcpy(vertices + (faces_added * 4 + j) * 5, vertex,
                   sizeof(float) * 5);
        }

        faces_added++;
    }
}

void block_init(block *block, vector3i position, block_type type,
                bool *active_faces, tilemap *tilemap) {
    block->position = position;
    block->type = type;
    block->tilemap = tilemap;

    if (type != BLOCK_TYPE_EMPTY) {
        memcpy(block->active_faces, active_faces, sizeof(bool) * 6);
    } else {
        /*bool all_inactive_faces[] = {false, false, false, false, false,
         * false};*/
        memcpy(block->active_faces, ACTIVE_FACES_NONE, sizeof(bool) * 6);
    }

    block_texture texture = block_type_to_texture(type);

    block->active_face_count = 0;

    for (int i = 0; i < 6; i++) {
        if (block->active_faces[i] == true) {
            block->active_face_count++;
        }
    }

    block->vertex_count = block->active_face_count * 4;
    block->index_count = block->active_face_count * 6;

    block->vertices = malloc(sizeof(float) * 5 * block->vertex_count);
    block->indices = malloc(sizeof(unsigned int) * block->index_count);

    generate_vertices(block, block->vertices, block->indices);
}

void block_term(block *block) {
    free(block->vertices);
    free(block->indices);
}
