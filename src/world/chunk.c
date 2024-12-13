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
    chunk->visible = true;
    chunk->generation_stage = 0;
    chunk->position = position;
    chunk->tilemap = tilemap;

    chunk->vao.gl_id = 0;
    chunk->vbo.gl_id = 0;
    chunk->ibo.gl_id = 0;

    pthread_mutex_init(&chunk->mesh_mutex, NULL);

    chunk_update_mesh(chunk);
    chunk_update_buffers(chunk);
}

void chunk_update_mesh(chunk *chunk) {
    pthread_mutex_lock(&chunk->mesh_mutex);
    for (int z = 0; z < CHUNK_SIZE_Z; z++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int x = 0; x < CHUNK_SIZE_X; x++) {
                if (chunk->blocks[z][y][x] != BLOCK_TYPE_EMPTY) {
                    for (int i = 0; i < 6; i++) {
                        if (is_block_face_active(chunk, x, y, z, i)) {
                            chunk->face_count++;
                        }
                    }
                }
            }
        }
    }

    chunk->vertices =
        malloc(chunk->face_count * 4 * CHUNK_VERTEX_SIZE * sizeof(float));
    chunk->indices = malloc(chunk->face_count * 6 * sizeof(unsigned int));

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
                        chunk->indices[faces_added * 6 + i] =
                            INDEX_ORDER[i] + faces_added * 4;
                    }

                    int face_index = (faces_added * 4) * CHUNK_VERTEX_SIZE;

                    for (int i = 0; i < 4; i++) {
                        int vertex_index = face_index + i * CHUNK_VERTEX_SIZE;

                        memcpy(chunk->vertices + vertex_index,
                               VERTEX_POSITIONS[FACE_INDICES[f][i]],
                               3 * sizeof(float));

                        // dont think blocks are positioned correctly as block
                        // origin back top left but chunk is back bottom left
                        chunk->vertices[vertex_index] +=
                            x + chunk->position.x * CHUNK_SIZE_X;
                        chunk->vertices[vertex_index + 1] +=
                            y + chunk->position.y * CHUNK_SIZE_Y;
                        chunk->vertices[vertex_index + 2] +=
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

                    chunk->vertices[vertex_1_index + 3] =
                        texture_rectangle.x; // save index to variable
                    chunk->vertices[vertex_1_index + 4] =
                        texture_rectangle.y + texture_rectangle.height;

                    chunk->vertices[vertex_2_index + 3] =
                        texture_rectangle.x + texture_rectangle.width;
                    chunk->vertices[vertex_2_index + 4] =
                        texture_rectangle.y + texture_rectangle.height;

                    chunk->vertices[vertex_3_index + 3] =
                        texture_rectangle.x + texture_rectangle.width;
                    chunk->vertices[vertex_3_index + 4] = texture_rectangle.y;

                    chunk->vertices[vertex_4_index + 3] = texture_rectangle.x;
                    chunk->vertices[vertex_4_index + 4] = texture_rectangle.y;

                    // Set vertex normals
                    chunk->vertices[vertex_1_index + 5] = FACE_NORMALS[f][0];
                    chunk->vertices[vertex_1_index + 6] = FACE_NORMALS[f][1];
                    chunk->vertices[vertex_1_index + 7] = FACE_NORMALS[f][2];

                    chunk->vertices[vertex_2_index + 5] = FACE_NORMALS[f][0];
                    chunk->vertices[vertex_2_index + 6] = FACE_NORMALS[f][1];
                    chunk->vertices[vertex_2_index + 7] = FACE_NORMALS[f][2];

                    chunk->vertices[vertex_3_index + 5] = FACE_NORMALS[f][0];
                    chunk->vertices[vertex_3_index + 6] = FACE_NORMALS[f][1];
                    chunk->vertices[vertex_3_index + 7] = FACE_NORMALS[f][2];

                    chunk->vertices[vertex_4_index + 5] = FACE_NORMALS[f][0];
                    chunk->vertices[vertex_4_index + 6] = FACE_NORMALS[f][1];
                    chunk->vertices[vertex_4_index + 7] = FACE_NORMALS[f][2];

                    faces_added++;
                }
            }
        }
    }
    pthread_mutex_unlock(&chunk->mesh_mutex);
}

void chunk_update_buffers(chunk *chunk) {
    pthread_mutex_lock(&chunk->mesh_mutex);
    vao_destroy(&chunk->vao);
    bo_destroy(&chunk->vbo);
    bo_destroy(&chunk->ibo);

    vao_init(&chunk->vao);
    bo_init(&chunk->vbo, BO_TYPE_VERTEX);
    bo_init(&chunk->ibo, BO_TYPE_INDEX);

    vao_bind(&chunk->vao);
    bo_bind(&chunk->vbo);
    bo_bind(&chunk->ibo);

    bo_upload(&chunk->vbo,
              chunk->face_count * 4 * CHUNK_VERTEX_SIZE * sizeof(float),
              chunk->vertices, BO_USAGE_STATIC_DRAW);

    bo_upload(&chunk->ibo, chunk->face_count * 6 * sizeof(unsigned int),
              chunk->indices, BO_USAGE_STATIC_DRAW);

    vao_attrib(&chunk->vao, 0, 3, VAO_TYPE_FLOAT, false,
               CHUNK_VERTEX_SIZE * sizeof(float), 0);
    vao_attrib(&chunk->vao, 1, 2, VAO_TYPE_FLOAT, false,
               CHUNK_VERTEX_SIZE * sizeof(float), (void *)(3 * sizeof(float)));
    vao_attrib(&chunk->vao, 2, 3, VAO_TYPE_FLOAT, false,
               CHUNK_VERTEX_SIZE * sizeof(float), (void *)(5 * sizeof(float)));
    pthread_mutex_unlock(&chunk->mesh_mutex);
}

// bind texture
void chunk_draw(chunk *chunk) {
    pthread_mutex_lock(&chunk->mesh_mutex);
    if (!chunk->visible) {
        pthread_mutex_unlock(&chunk->mesh_mutex);
        return;
    }

    vao_bind(&chunk->vao);
    bo_bind(&chunk->ibo);
    bo_bind(&chunk->vbo);

    int index_count = bo_get_size(&chunk->ibo) / sizeof(unsigned int);

    renderer_draw_elements(DRAW_MODE_TRIANGLES, index_count,
                           INDEX_TYPE_UNSIGNED_INT);
    pthread_mutex_unlock(&chunk->mesh_mutex);
}
