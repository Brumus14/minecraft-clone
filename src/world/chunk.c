#include "chunk.h"

#include "string.h"

bool is_block_face_active(chunk *chunk, int x, int y, int z, block_face face) {
    // XRAY MODE
    if (chunk->blocks[z][y][x] == BLOCK_TYPE_COAL) {
        switch (face) {
        case BLOCK_FACE_FRONT:
            return !(z < CHUNK_SIZE_Z - 1 &&
                     chunk->blocks[z + 1][y][x] == BLOCK_TYPE_COAL);
        case BLOCK_FACE_TOP:
            return !(y < CHUNK_SIZE_Y - 1 &&
                     chunk->blocks[z][y + 1][x] == BLOCK_TYPE_COAL);
        case BLOCK_FACE_RIGHT:
            return !(x < CHUNK_SIZE_X - 1 &&
                     chunk->blocks[z][y][x + 1] == BLOCK_TYPE_COAL);
        case BLOCK_FACE_BOTTOM:
            return !(y > 0 && chunk->blocks[z][y - 1][x] == BLOCK_TYPE_COAL);
        case BLOCK_FACE_LEFT:
            return !(x > 0 && chunk->blocks[z][y][x - 1] == BLOCK_TYPE_COAL);
        case BLOCK_FACE_BACK:
            return !(z > 0 && chunk->blocks[z - 1][y][x] == BLOCK_TYPE_COAL);
        }
    }

    if (chunk->blocks[z][y][x] == BLOCK_TYPE_DIAMOND) {
        switch (face) {
        case BLOCK_FACE_FRONT:
            return !(z < CHUNK_SIZE_Z - 1 &&
                     chunk->blocks[z + 1][y][x] == BLOCK_TYPE_DIAMOND);
        case BLOCK_FACE_TOP:
            return !(y < CHUNK_SIZE_Y - 1 &&
                     chunk->blocks[z][y + 1][x] == BLOCK_TYPE_DIAMOND);
        case BLOCK_FACE_RIGHT:
            return !(x < CHUNK_SIZE_X - 1 &&
                     chunk->blocks[z][y][x + 1] == BLOCK_TYPE_DIAMOND);
        case BLOCK_FACE_BOTTOM:
            return !(y > 0 && chunk->blocks[z][y - 1][x] == BLOCK_TYPE_DIAMOND);
        case BLOCK_FACE_LEFT:
            return !(x > 0 && chunk->blocks[z][y][x - 1] == BLOCK_TYPE_DIAMOND);
        case BLOCK_FACE_BACK:
            return !(z > 0 && chunk->blocks[z - 1][y][x] == BLOCK_TYPE_DIAMOND);
        }
    }

    switch (face) {
    case BLOCK_FACE_FRONT:
        return !(z < CHUNK_SIZE_Z - 1 &&
                 chunk->blocks[z + 1][y][x] != BLOCK_TYPE_EMPTY);
    case BLOCK_FACE_TOP:
        return !(y < CHUNK_SIZE_Y - 1 &&
                 chunk->blocks[z][y + 1][x] != BLOCK_TYPE_EMPTY);
    case BLOCK_FACE_RIGHT:
        return !(x < CHUNK_SIZE_X - 1 &&
                 chunk->blocks[z][y][x + 1] != BLOCK_TYPE_EMPTY);
    case BLOCK_FACE_BOTTOM:
        return !(y > 0 && chunk->blocks[z][y - 1][x] != BLOCK_TYPE_EMPTY);
    case BLOCK_FACE_LEFT:
        return !(x > 0 && chunk->blocks[z][y][x - 1] != BLOCK_TYPE_EMPTY);
    case BLOCK_FACE_BACK:
        return !(z > 0 && chunk->blocks[z - 1][y][x] != BLOCK_TYPE_EMPTY);
    }

    return false;
}

void chunk_init(chunk *chunk, vector3i position, tilemap *tilemap) {
    // chunk->visible = true;
    chunk->status = CHUNK_STATUS_UNGENERATED;
    chunk->position = position;
    chunk->tilemap = tilemap;

    chunk->vao.gl_id = 0;
    chunk->vbo.gl_id = 0;
    chunk->ibo.gl_id = 0;

    chunk_update(chunk);
}

void chunk_update(chunk *chunk) {
    vao_destroy(&chunk->vao);
    bo_destroy(&chunk->vbo);
    bo_destroy(&chunk->ibo);

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
                    for (int i = 0; i < 6; i++) {
                        if (is_block_face_active(chunk, x, y, z, i)) {
                            faces_active++;
                        }
                    }
                }
            }
        }
    }

    float *vertices =
        malloc(faces_active * 4 * CHUNK_VERTEX_SIZE * sizeof(float));
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

                    int face_index = (faces_added * 4) * CHUNK_VERTEX_SIZE;

                    for (int i = 0; i < 4; i++) {
                        int vertex_index = face_index + i * CHUNK_VERTEX_SIZE;

                        memcpy(vertices + vertex_index,
                               VERTEX_POSITIONS[FACE_INDICES[f][i]],
                               3 * sizeof(float));

                        // dont think blocks are positioned correctly as block
                        // origin back top left but chunk is back bottom left
                        vertices[vertex_index] +=
                            x + chunk->position.x * CHUNK_SIZE_X;
                        vertices[vertex_index + 1] +=
                            y + chunk->position.y * CHUNK_SIZE_Y;
                        vertices[vertex_index + 2] +=
                            z + chunk->position.z * CHUNK_SIZE_Z;
                    }

                    rectangle texture_rectangle = tilemap_get_tile_rectangle(
                        chunk->tilemap,
                        block_type_to_texture(chunk->blocks[z][y][x])
                            .face_texture_indices[f]);

                    int vertex_1_index = face_index;
                    int vertex_2_index = face_index + CHUNK_VERTEX_SIZE;
                    int vertex_3_index = face_index + 2 * CHUNK_VERTEX_SIZE;
                    int vertex_4_index = face_index + 3 * CHUNK_VERTEX_SIZE;

                    vertices[vertex_1_index + 3] =
                        texture_rectangle.x; // save index to variable
                    vertices[vertex_1_index + 4] =
                        texture_rectangle.y + texture_rectangle.height;

                    vertices[vertex_2_index + 3] =
                        texture_rectangle.x + texture_rectangle.width;
                    vertices[vertex_2_index + 4] =
                        texture_rectangle.y + texture_rectangle.height;

                    vertices[vertex_3_index + 3] =
                        texture_rectangle.x + texture_rectangle.width;
                    vertices[vertex_3_index + 4] = texture_rectangle.y;

                    vertices[vertex_4_index + 3] = texture_rectangle.x;
                    vertices[vertex_4_index + 4] = texture_rectangle.y;

                    // Set vertex normals
                    vertices[vertex_1_index + 5] = FACE_NORMALS[f][0];
                    vertices[vertex_1_index + 6] = FACE_NORMALS[f][1];
                    vertices[vertex_1_index + 7] = FACE_NORMALS[f][2];

                    vertices[vertex_2_index + 5] = FACE_NORMALS[f][0];
                    vertices[vertex_2_index + 6] = FACE_NORMALS[f][1];
                    vertices[vertex_2_index + 7] = FACE_NORMALS[f][2];

                    vertices[vertex_3_index + 5] = FACE_NORMALS[f][0];
                    vertices[vertex_3_index + 6] = FACE_NORMALS[f][1];
                    vertices[vertex_3_index + 7] = FACE_NORMALS[f][2];

                    vertices[vertex_4_index + 5] = FACE_NORMALS[f][0];
                    vertices[vertex_4_index + 6] = FACE_NORMALS[f][1];
                    vertices[vertex_4_index + 7] = FACE_NORMALS[f][2];

                    faces_added++;
                }
            }
        }
    }

    bo_upload(&chunk->vbo, faces_active * 4 * CHUNK_VERTEX_SIZE * sizeof(float),
              vertices, BO_USAGE_STATIC_DRAW);

    bo_upload(&chunk->ibo, faces_active * 6 * sizeof(unsigned int), indices,
              BO_USAGE_STATIC_DRAW);

    vao_attrib(&chunk->vao, 0, 3, VAO_TYPE_FLOAT, false,
               CHUNK_VERTEX_SIZE * sizeof(float), 0);
    vao_attrib(&chunk->vao, 1, 2, VAO_TYPE_FLOAT, false,
               CHUNK_VERTEX_SIZE * sizeof(float), (void *)(3 * sizeof(float)));
    vao_attrib(&chunk->vao, 2, 3, VAO_TYPE_FLOAT, false,
               CHUNK_VERTEX_SIZE * sizeof(float), (void *)(5 * sizeof(float)));
}

// bind texture
void chunk_draw(chunk *chunk) {
    if (chunk->status != CHUNK_STATUS_DONE) {
        return;
    }

    vao_bind(&chunk->vao);
    bo_bind(&chunk->ibo);
    bo_bind(&chunk->vbo);

    int index_count = bo_get_size(&chunk->ibo) / sizeof(unsigned int);

    renderer_draw_elements(DRAW_MODE_TRIANGLES, index_count,
                           INDEX_TYPE_UNSIGNED_INT);
}
