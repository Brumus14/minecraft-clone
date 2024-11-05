#include "chunk.h"

#include <string.h>

void chunk_init(chunk *chunk, vector3 position, tilemap *tilemap) {
    chunk->position = position;
    chunk->tilemap = tilemap;

    for (int z = 0; z < CHUNK_SIZE_Z; z++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int x = 0; x < CHUNK_SIZE_X; x++) {
                block_type type = BLOCK_TYPE_EMPTY;

                if (y == CHUNK_SIZE_Y - 1) {
                    type = BLOCK_TYPE_GRASS;
                } else {
                    type = BLOCK_TYPE_DIRT;
                }

                /*if (y - (15 * cos((x * M_PI / 2) / 15)) > 0.01) {*/
                /*    type = BLOCK_TYPE_EMPTY;*/
                /*}*/

                block_init(&chunk->blocks[z][y][x], (vector3){x, y, z}, type,
                           ACTIVE_FACES_ALL, chunk->tilemap);
            }
        }
    }
}

// one draw call for the chunk
void chunk_calculate_active_faces(chunk *chunk) {
    for (int z = 0; z < CHUNK_SIZE_Z; z++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int x = 0; x < CHUNK_SIZE_X; x++) {
                block *block = &chunk->blocks[z][y][x];

                if (block->type == BLOCK_TYPE_EMPTY) {
                    continue;
                }

                memcpy(block->active_faces, ACTIVE_FACES_ALL, sizeof(bool) * 6);

                if (x < CHUNK_SIZE_X - 1 &&
                    chunk->blocks[z][y][x + 1].type != BLOCK_TYPE_EMPTY) {
                    block->active_faces[2] = false;
                }

                if (x > 0 &&
                    chunk->blocks[z][y][x - 1].type != BLOCK_TYPE_EMPTY) {
                    block->active_faces[4] = false;
                }

                if (y < CHUNK_SIZE_Y - 1 &&
                    chunk->blocks[z][y + 1][x].type != BLOCK_TYPE_EMPTY) {
                    block->active_faces[1] = false;
                }

                if (y > 0 &&
                    chunk->blocks[z][y - 1][x].type != BLOCK_TYPE_EMPTY) {
                    block->active_faces[3] = false;
                }

                if (z < CHUNK_SIZE_Z - 1 &&
                    chunk->blocks[z + 1][y][x].type != BLOCK_TYPE_EMPTY) {
                    block->active_faces[0] = false;
                }

                if (z > 0 &&
                    chunk->blocks[z - 1][y][x].type != BLOCK_TYPE_EMPTY) {
                    block->active_faces[5] = false;
                }

                block_term(block);

                block_init(block, block->position, block->type,
                           block->active_faces, block->tilemap);
            }
        }
    }
}

void chunk_generate_vertices_indices(chunk *chunk) {
    int total_vertex_count = 0;
    int total_index_count = 0;

    for (int z = 0; z < CHUNK_SIZE_Z; z++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int x = 0; x < CHUNK_SIZE_X; x++) {
                block *block = &chunk->blocks[z][y][x];

                total_vertex_count += block->active_face_count * 4;
                total_index_count += block->active_face_count * 6;
            }
        }
    }

    float *vertices = malloc(sizeof(float) * 5 * total_vertex_count);
    unsigned int *indices = malloc(sizeof(int) * total_index_count);

    int vertices_added = 0;
    int indices_added = 0;

    for (int z = 0; z < CHUNK_SIZE_Z; z++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int x = 0; x < CHUNK_SIZE_X; x++) {
                block *block = &chunk->blocks[z][y][x];

                memcpy(vertices + (5 * vertices_added), block->vertices,
                       sizeof(float) * 5 * block->vertex_count);
                memcpy(indices + indices_added, block->indices,
                       sizeof(unsigned int) * block->index_count);

                for (int i = 0; i < block->index_count; i++) {
                    indices[indices_added + i] += vertices_added;
                }

                vertices_added += block->vertex_count;
                indices_added += block->index_count;
            }
        }
    }

    for (int i = 0; i < total_vertex_count; i++) {
        vertices[i * 5] += chunk->position.x;
        vertices[i * 5 + 1] += chunk->position.y;
        vertices[i * 5 + 2] += chunk->position.z;
    }

    bo_init(&chunk->vbo, BO_TYPE_VERTEX);
    bo_bind(&chunk->vbo);
    bo_upload(&chunk->vbo, sizeof(float) * 5 * total_vertex_count, vertices,
              BO_USAGE_STATIC_DRAW);

    bo_init(&chunk->ibo, BO_TYPE_INDEX);
    bo_bind(&chunk->ibo);
    bo_upload(&chunk->ibo, sizeof(int) * total_index_count, indices,
              BO_USAGE_STATIC_DRAW);

    vao_init(&chunk->vao);
    vao_bind(&chunk->vao);
    vao_attrib(&chunk->vao, 0, 3, VAO_TYPE_FLOAT, false, 5 * sizeof(float), 0);
    vao_attrib(&chunk->vao, 1, 2, VAO_TYPE_FLOAT, false, 5 * sizeof(float),
               (void *)(3 * sizeof(float)));

    free(vertices);
    free(indices);
}

void chunk_draw(chunk *chunk) {
    int total_index_count = 0;

    for (int z = 0; z < CHUNK_SIZE_Z; z++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int x = 0; x < CHUNK_SIZE_X; x++) {
                block *block = &chunk->blocks[z][y][x];

                total_index_count += block->active_face_count * 6;
            }
        }
    }

    tilemap_bind(chunk->tilemap);
    bo_bind(&chunk->vbo);
    bo_bind(&chunk->ibo);
    vao_bind(&chunk->vao);
    glDrawElements(GL_TRIANGLES, total_index_count, GL_UNSIGNED_INT, 0);
}
