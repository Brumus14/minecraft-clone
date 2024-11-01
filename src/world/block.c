#include "block.h"

#include <string.h>

block_texture block_type_to_texture(block_type type) {
    // front, top, right, bottom, left, back
    switch (type) {
    case BLOCK_TYPE_EMPTY:
        return (block_texture){
            .empty = true, .face_textures = {0, 0, 0, 0, 0, 0}
        };
    case BLOCK_TYPE_GRASS:
        return (block_texture){
            .empty = false, .face_textures = {1, 0, 1, 2, 1, 1}
        };
    case BLOCK_TYPE_DIRT:
        return (block_texture){
            .empty = false, .face_textures = {3, 2, 3, 2, 3, 3}
        };
    }
}

int count = 0;
float *generate_vertices(block *block, float *vertices, int *indices) {
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

    int active_faces = 0;

    for (int i = 0; i < 6; i++) {
        if (block->active_faces[i] == true) {
            active_faces++;
        }
    }

    memcpy(indices,
           (int[]){
               0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,
               8,  9,  10, 8,  10, 11, 12, 13, 14, 12, 14, 15,
               16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23,
           },
           sizeof(int) * 6 * 6);

    int faces_added = 0;

    for (int i = 0; i < 6; i++) {
        if (block->active_faces[i] == false) {
            /*printf("%d: %f, %f, %f\n", i, block->position.x,
             * block->position.y,*/
            /*       block->position.z);*/
            continue;
        }

        // sizeof somewhere?
        /*memcpy(indices + faces_added * 6, , sizeof(int) * 6);*/
        /*indices[faces_added * 6] = 0 + (faces_added * 4);*/
        /*indices[faces_added * 6 + 1] = 1 + (faces_added * 4);*/
        /*indices[faces_added * 6 + 2] = 2 + (faces_added * 4);*/
        /*indices[faces_added * 6 + 3] = 0 + (faces_added * 4);*/
        /*indices[faces_added * 6 + 4] = 2 + (faces_added * 4);*/
        /*indices[faces_added * 6 + 5] = 3 + (faces_added * 4);*/

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

    return vertices;
}

void block_init(block *block, vector3 position, block_type type,
                bool *active_faces, tilemap *tilemap) {
    block->position = position;
    block->type = type;
    block->tilemap = tilemap;

    memcpy(block->active_faces, active_faces, sizeof(bool) * 6);

    block_texture texture = block_type_to_texture(type);

    int active_face_count = 0;

    for (int i = 0; i < 6; i++) {
        if (block->active_faces[i] == true) {
            active_face_count++;
        }
    }

    float *vertices = malloc(sizeof(float) * 5 * active_face_count * 4);
    int *indices = malloc(sizeof(int) * 6 * 6);

    generate_vertices(block, vertices, indices);

    /*memcpy(indices,*/
    /*       (int[]){*/
    /*           0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,*/
    /*           8,  9,  10, 8,  10, 11, 12, 13, 14, 12, 14, 15,*/
    /*           16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23,*/
    /*       },*/
    /*       sizeof(int) * 6 * 6 - 3);*/

    /*for (int i = 0; i < 5 * active_face_count * 4; i++) {*/
    /*    printf("%f, ", vertices[i]);*/
    /**/
    /*    if ((i + 1) % 5 == 0) {*/
    /*        putchar('\n');*/
    /*    }*/
    /**/
    /*    if ((i + 1) % 20 == 0) {*/
    /*        putchar('\n');*/
    /*    }*/
    /*}*/

    for (int i = 0; i < active_face_count * 6; i++) {
        printf("%d, ", indices[i]);

        if ((i + 1) % 6 == 0) {
            putchar('\n');
        }
    }

    printf("------------------------------------------------------\n");
    printf("%d\n", active_face_count * 6);

    bo_init(&block->vbo, BO_TYPE_VERTEX);
    bo_bind(&block->vbo);
    bo_upload(&block->vbo, sizeof(float) * 5 * active_face_count * 4, vertices,
              BO_USAGE_STATIC_DRAW);

    bo_init(&block->ibo, BO_TYPE_INDEX);
    bo_bind(&block->ibo);
    bo_upload(&block->ibo, sizeof(int) * 6 * 6, indices, BO_USAGE_STATIC_DRAW);

    vao_init(&block->vao);
    vao_bind(&block->vao);
    vao_attrib(&block->vao, 0, 3, VAO_TYPE_FLOAT, false, 5 * sizeof(float), 0);
    vao_attrib(&block->vao, 1, 2, VAO_TYPE_FLOAT, false, 5 * sizeof(float),
               (void *)(3 * sizeof(float)));

    count++;
}

void block_draw(block *block) {
    block_texture texture = block_type_to_texture(block->type);

    if (texture.empty) {
        return;
    }

    int active_face_count = 0;

    for (int i = 0; i < 6; i++) {
        if (block->active_faces[i] == true) {
            active_face_count++;
        }
    }

    tilemap_bind(block->tilemap);
    bo_bind(&block->vbo);
    bo_bind(&block->ibo);
    vao_bind(&block->vao);

    // use renderer calls
    glDrawElements(GL_TRIANGLES, active_face_count * 6, GL_UNSIGNED_INT, 0);
}
