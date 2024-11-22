#include "chunk.h"

#include "string.h"

bool is_block_face_active(chunk *chunk, int x, int y, int z, int face) {
    // XRAY MODE
    /*if (chunk->blocks[z][y][x] == BLOCK_TYPE_COAL) {*/
    /*    switch (face) {*/
    /*    case 0:*/
    /*        return !(z < CHUNK_SIZE_Z - 1 &&*/
    /*                 chunk->blocks[z + 1][y][x] == BLOCK_TYPE_COAL);*/
    /*    case 1:*/
    /*        return !(y < CHUNK_SIZE_Y - 1 &&*/
    /*                 chunk->blocks[z][y + 1][x] == BLOCK_TYPE_COAL);*/
    /*    case 2:*/
    /*        return !(x < CHUNK_SIZE_X - 1 &&*/
    /*                 chunk->blocks[z][y][x + 1] == BLOCK_TYPE_COAL);*/
    /*    case 3:*/
    /*        return !(y > 0 && chunk->blocks[z][y - 1][x] ==
     * BLOCK_TYPE_COAL);*/
    /*    case 4:*/
    /*        return !(x > 0 && chunk->blocks[z][y][x - 1] ==
     * BLOCK_TYPE_COAL);*/
    /*    case 5:*/
    /*        return !(z > 0 && chunk->blocks[z - 1][y][x] ==
     * BLOCK_TYPE_COAL);*/
    /*    }*/
    /*}*/

    switch (face) {
    case 0:
        return !(z < CHUNK_SIZE_Z - 1 &&
                 chunk->blocks[z + 1][y][x] != BLOCK_TYPE_EMPTY);
    case 1:
        return !(y < CHUNK_SIZE_Y - 1 &&
                 chunk->blocks[z][y + 1][x] != BLOCK_TYPE_EMPTY);
    case 2:
        return !(x < CHUNK_SIZE_X - 1 &&
                 chunk->blocks[z][y][x + 1] != BLOCK_TYPE_EMPTY);
    case 3:
        return !(y > 0 && chunk->blocks[z][y - 1][x] != BLOCK_TYPE_EMPTY);
    case 4:
        return !(x > 0 && chunk->blocks[z][y][x - 1] != BLOCK_TYPE_EMPTY);
    case 5:
        return !(z > 0 && chunk->blocks[z - 1][y][x] != BLOCK_TYPE_EMPTY);
    }

    return false;
}

void chunk_init(chunk *chunk, vector3i position, tilemap *tilemap) {
    chunk->position = position;
    chunk->tilemap = tilemap;

    chunk->vao.gl_id = 0;
    chunk->vbo.gl_id = 0;
    chunk->ibo.gl_id = 0;

    chunk_update(chunk);
}

void chunk_update(chunk *chunk) {
    vao_delete(&chunk->vao);
    bo_delete(&chunk->vbo);
    bo_delete(&chunk->ibo);

    vao_init(&chunk->vao);
    bo_init(&chunk->vbo, BO_TYPE_VERTEX);
    bo_init(&chunk->ibo, BO_TYPE_INDEX);

    vao_bind(&chunk->vao);
    bo_bind(&chunk->vbo);
    bo_bind(&chunk->ibo);

    int faces_active = 0;

    for (int z = 0; z < CHUNK_SIZE_Z; z++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int x = 0; x < CHUNK_SIZE_X; x++) {
                if (chunk->blocks[z][y][x] != BLOCK_TYPE_EMPTY) {
                    // maybe save these active faces to use later
                    for (int i = 0; i < 6; i++) {
                        if (is_block_face_active(chunk, x, y, z, i)) {
                            faces_active++;
                        }
                    }
                }
            }
        }
    }

    float *vertices = malloc(faces_active * 4 * 5 * sizeof(float));
    unsigned int *indices = malloc(faces_active * 6 * sizeof(unsigned int));

    int faces_added = 0;

    for (int z = 0; z < CHUNK_SIZE_Z; z++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int x = 0; x < CHUNK_SIZE_X; x++) {
                if (chunk->blocks[z][y][x] == BLOCK_TYPE_EMPTY) {
                    continue;
                }

                for (int f = 0; f < 6; f++) {
                    if (!is_block_face_active(chunk, x, y, z, f)) {
                        continue;
                    }

                    for (int i = 0; i < 6; i++) {
                        indices[faces_added * 6 + i] =
                            INDEX_ORDER[i] + faces_added * 4;
                    }

                    for (int i = 0; i < 4; i++) {
                        memcpy(vertices + (faces_added * 4 + i) * 5,
                               VERTEX_POSITIONS[FACE_INDICES[f][i]],
                               3 * sizeof(float));

                        // dont think blocks are positioned correctly as block
                        // origin back top left but chunk is back bottom left
                        vertices[(faces_added * 4 + i) * 5] +=
                            x + chunk->position.x * CHUNK_SIZE_X;
                        vertices[(faces_added * 4 + i) * 5 + 1] +=
                            y + chunk->position.y * CHUNK_SIZE_Y;
                        vertices[(faces_added * 4 + i) * 5 + 2] +=
                            z + chunk->position.z * CHUNK_SIZE_Z;
                    }

                    rectangle texture_rectangle = tilemap_get_tile_rectangle(
                        chunk->tilemap,
                        block_type_to_texture(chunk->blocks[z][y][x])
                            .face_texture_indices[f]);

                    vertices[(faces_added * 4) * 5 + 3] =
                        texture_rectangle.x; // save index to variable
                    vertices[(faces_added * 4) * 5 + 4] =
                        texture_rectangle.y + texture_rectangle.height;

                    vertices[(faces_added * 4 + 1) * 5 + 3] =
                        texture_rectangle.x + texture_rectangle.width;
                    vertices[(faces_added * 4 + 1) * 5 + 4] =
                        texture_rectangle.y + texture_rectangle.height;

                    vertices[(faces_added * 4 + 2) * 5 + 3] =
                        texture_rectangle.x + texture_rectangle.width;
                    vertices[(faces_added * 4 + 2) * 5 + 4] =
                        texture_rectangle.y;

                    vertices[(faces_added * 4 + 3) * 5 + 3] =
                        texture_rectangle.x;
                    vertices[(faces_added * 4 + 3) * 5 + 4] =
                        texture_rectangle.y;

                    faces_added++;
                }
            }
        }
    }

    bo_upload(&chunk->vbo, faces_active * 4 * 5 * sizeof(float), vertices,
              BO_USAGE_STATIC_DRAW);

    bo_upload(&chunk->ibo, faces_active * 6 * sizeof(unsigned int), indices,
              BO_USAGE_STATIC_DRAW);

    vao_attrib(&chunk->vao, 0, 3, VAO_TYPE_FLOAT, false, 5 * sizeof(float), 0);
    vao_attrib(&chunk->vao, 1, 2, VAO_TYPE_FLOAT, false, 5 * sizeof(float),
               (void *)(3 * sizeof(float)));
}

void chunk_draw(chunk *chunk) {
    vao_bind(&chunk->vao);
    bo_bind(&chunk->ibo);
    bo_bind(&chunk->vbo);

    int index_count = bo_get_size(&chunk->ibo) / sizeof(unsigned int);

    renderer_draw_elements(DRAW_MODE_TRIANGLES, index_count,
                           INDEX_TYPE_UNSIGNED_INT);
}
