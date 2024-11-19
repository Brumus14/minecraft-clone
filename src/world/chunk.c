#include "chunk.h"

void chunk_update(chunk *chunk) {
    float vertices[][5] = {
        5,  5,  0, // top right
        5,  -5, 0, // bottom right
        -5, -5, 0, // bottom left
        -5, 5,  0  // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    vao_delete(&chunk->vao);
    bo_delete(&chunk->vbo);
    bo_delete(&chunk->ibo);

    vao_init(&chunk->vao);
    bo_init(&chunk->vbo, BO_TYPE_VERTEX);
    bo_init(&chunk->ibo, BO_TYPE_INDEX);

    vao_bind(&chunk->vao);

    bo_bind(&chunk->vbo);
    bo_upload(&chunk->vbo, 12 * sizeof(float), vertices, BO_USAGE_STATIC_DRAW);

    bo_bind(&chunk->ibo);
    bo_upload(&chunk->ibo, 6 * sizeof(unsigned int), indices,
              BO_USAGE_STATIC_DRAW);

    vao_attrib(&chunk->vao, 0, 3, VAO_TYPE_FLOAT, false, 3 * sizeof(float), 0);

    /*vao_attrib(&chunk->vao, 0, 3, VAO_TYPE_FLOAT, false, 5 * sizeof(float),
     * 0);*/
    /*vao_attrib(&chunk->vao, 1, 2, VAO_TYPE_FLOAT, false, 5 * sizeof(float),*/
    /*           (void *)(3 * sizeof(float)));*/
}

void chunk_init(chunk *chunk, vector3i position, tilemap *tilemap) {
    chunk->position = position;
    chunk->tilemap = tilemap;

    chunk->vao.gl_id = 0;
    chunk->vbo.gl_id = 0;
    chunk->ibo.gl_id = 0;

    chunk_update(chunk);
}

void chunk_draw(chunk *chunk) {
    vao_bind(&chunk->vao);
    bo_bind(&chunk->ibo);
    bo_bind(&chunk->vbo);

    renderer_draw_elements(DRAW_MODE_TRIANGLES, 6, INDEX_TYPE_UNSIGNED_INT);
}
