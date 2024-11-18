#include "chunk.h"

#include <string.h>
#include "../graphics/renderer.h"

void chunk_init(chunk *chunk, vector3i position, tilemap *tilemap) {
    chunk->position = position;
    chunk->tilemap = tilemap;

    for (int z = 0; z < CHUNK_SIZE_Z; z++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int x = 0; x < CHUNK_SIZE_X; x++) {
                block_init(&chunk->blocks[z][y][x], (vector3i){x, y, z},
                           BLOCK_TYPE_EMPTY, ACTIVE_FACES_ALL, chunk->tilemap);
            }
        }
    }
}

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

                vector3i block_position;
                vector3i_init(&block_position, x, y, z);

                block_init(block, block_position, block->type,
                           block->active_faces, block->tilemap);
            }
        }
    }
}
/*float *vertices, unsigned int *indices*/
void greedy_mesh_vertices_indices() {
    int blocks[4][5] = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 0, 1},
        {1, 0, 0, 0, 1}
    };

    int total_vertex_count = 0;

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 5; x++) {
            if (blocks[y][x] == 1) {
                total_vertex_count += 4;
            }
        }
    }

    float *total_vertices = malloc(sizeof(float) * 5 * total_vertex_count);

    int vertices_added = 0;

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 5; x++) {
            if (blocks[y][x] == 1) {
                float vertex1[5] = {x, y, 0.0, 0.0, 0.0};
                float positions[4][3] = {
                    {0.0, -1.0, 0.0},
                    {1.0, -1.0, 0.0},
                    {1.0, 0.0,  0.0},
                    {0.0, 0.0,  0.0}
                };

                for (int i = 0; i < 4; i++) {
                    memcpy(total_vertices + vertices_added * 5, vertex1,
                           sizeof(float) * 5);

                    total_vertices[vertices_added] += positions[i][0];
                    total_vertices[vertices_added + 1] += positions[i][1];
                    total_vertices[vertices_added + 2] += positions[i][2];

                    vertices_added++;
                }
            }
        }
    }

    float *vertices = malloc(sizeof(float) * 5 * total_vertex_count);

    float start_pos[2] = {0.0, 0.0};
    int current_v = 0;

    for (int v = 0; v < total_vertex_count; v++) {
        memcpy(vertices, total_vertices, 5);

        start_pos[0] = total_vertices[v * 5];
        start_pos[1] = total_vertices[v * 5 + 1];

        for (int i = current_v; i < total_vertex_count; i++) {
            if (total_vertices[i * 5] == total_vertices[v * 5] + 1) {
            }
        }
    }

    /*for (int i = 0; i < 5 * total_vertex_count; i++) {*/
    /*    printf("%f, ", total_vertices[i]);*/
    /**/
    /*    if ((i + 1) % 5 == 0) {*/
    /*        putchar('\n');*/
    /*    }*/
    /**/
    /*    if ((i + 1) % 20 == 0) {*/
    /*        putchar('\n');*/
    /*    }*/
    /*}*/
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
        vertices[i * 5] += chunk->position.x * 16;
        vertices[i * 5 + 1] += chunk->position.y * 16;
        vertices[i * 5 + 2] += chunk->position.z * 16;
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
    renderer_draw_elements(DRAW_MODE_TRIANGLES, total_index_count,
                           INDEX_TYPE_UNSIGNED_INT);
}
