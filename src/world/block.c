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

float *generate_vertices(block *block, float *vertices, int *indices) {
}

void block_init(block *block, vector3 position, block_type type,
                bool *active_faces, tilemap *tilemap) {
    block->position = position;
    block->type = type;
    block->tilemap = tilemap;

    memcpy(block->active_faces, active_faces, sizeof(bool) * 6);

    int active_face_count = 0;

    for (int i = 0; i < 6; i++) {
        if (block->active_faces[i] == true) {
            active_face_count++;
        }
    }

    float vertices[][5] = {
        {0.0, -1.0, 0.0, 0.0, 1.0},
        {1.0, -1.0, 0.0, 1.0, 1.0},
        {1.0, 0.0,  0.0, 1.0, 0.0},
        {0.0, 0.0,  0.0, 0.0, 0.0},
    };

    for (int i = 0; i < 4; i++) {
        vertices[i][0] += block->position.x;
        vertices[i][1] += block->position.y;
        vertices[i][2] += block->position.z;
    }

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};
    /*unsigned int *indices =*/
    /*    malloc(sizeof(unsigned int) * (active_face_count == 6 ? 6 : 3));*/
    /**/
    /*if (active_face_count == 6) {*/
    /*    indices[0] = 0;*/
    /*    indices[1] = 1;*/
    /*    indices[2] = 2;*/
    /*    indices[3] = 0;*/
    /*    indices[4] = 2;*/
    /*    indices[5] = 3;*/
    /*} else if (active_face_count == 5) {*/
    /*    indices[0] = 0;*/
    /*    indices[1] = 1;*/
    /*    indices[2] = 2;*/
    /*}*/

    for (int i = 0; i < (active_face_count == 6 ? 6 : 3); i++) {
        printf("Index %d: %u\n", i, indices[i]);
    }
    for (int i = 0; i < 4; i++) {
        printf("Vertex %d: %f, %f, %f, %f, %f\n", i, vertices[i][0],
               vertices[i][1], vertices[i][2], vertices[i][3], vertices[i][4]);
    }

    bo_init(&block->vbo, BO_TYPE_VERTEX);
    bo_upload(&block->vbo, sizeof(float) * 5 * 4, vertices,
              BO_USAGE_STATIC_DRAW);

    bo_init(&block->ibo, BO_TYPE_INDEX);
    bo_upload(&block->ibo, sizeof(unsigned int) * 6, indices,
              BO_USAGE_STATIC_DRAW);

    vao_init(&block->vao);
    vao_attrib(&block->vao, 0, 3, VAO_TYPE_FLOAT, false, sizeof(float) * 5,
               (void *)0);
    vao_attrib(&block->vao, 1, 2, VAO_TYPE_FLOAT, false, sizeof(float) * 5,
               (void *)(sizeof(float) * 3));

    /*free(indices);*/
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
    glDrawElements(GL_TRIANGLES, (active_face_count == 6 ? 6 : 3),
                   GL_UNSIGNED_INT, 0);
}
