#include "chunk.h"

#include <string.h>

void chunk_init(chunk *chunk, tilemap *tilemap) {
    chunk->tilemap = tilemap;

    for (int z = 0; z < 16; z++) {
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {
                block_type type;

                if (y == 15) {
                    type = BLOCK_TYPE_GRASS;
                } else {
                    type = BLOCK_TYPE_DIRT;
                }

                if (y - (15 * cos((x * M_PI / 2) / 15)) > 0.01) {
                    type = BLOCK_TYPE_EMPTY;
                }

                block_init(&chunk->blocks[z][y][x], (vector3){x, y, z}, type,
                           ACTIVE_FACES_ALL, chunk->tilemap);
            }
        }
    }
}

// one draw call for the chunk
void chunk_calculate_active_faces(chunk *chunk) {
    for (int z = 0; z < 16; z++) {
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {
                block *block = &chunk->blocks[z][y][x];

                if (block->type == BLOCK_TYPE_EMPTY) {
                    continue;
                }

                memcpy(block->active_faces, ACTIVE_FACES_ALL, sizeof(bool) * 6);

                if (x == 8) {
                    block->active_faces[2] = false;
                }

                /*if (x < 15 &&*/
                /*    chunk->blocks[z][y][x + 1].type != BLOCK_TYPE_EMPTY) {*/
                /*    block->active_faces[2] = false;*/
                /*}*/

                /*if (x > 0 &&*/
                /*    chunk->blocks[z][y][x - 1].type != BLOCK_TYPE_EMPTY) {*/
                /*    block->active_faces[4] = false;*/
                /*}*/

                /*if (y < 15 &&*/
                /*    chunk->blocks[z][y + 1][x].type != BLOCK_TYPE_EMPTY) {*/
                /*    block->active_faces[1] = false;*/
                /*}*/
                /**/
                /*if (y > 0 &&*/
                /*    chunk->blocks[z][y - 1][x].type != BLOCK_TYPE_EMPTY) {*/
                /*    block->active_faces[3] = false;*/
                /*}*/
                /**/
                /*if (z < 15 &&*/
                /*    chunk->blocks[z + 1][y][x].type != BLOCK_TYPE_EMPTY) {*/
                /*    block->active_faces[0] = false;*/
                /*}*/
                /**/
                /*if (z > 0 &&*/
                /*    chunk->blocks[z - 1][y][x].type != BLOCK_TYPE_EMPTY) {*/
                /*    block->active_faces[5] = false;*/
                /*}*/

                block_init(block, block->position, block->type,
                           block->active_faces, block->tilemap);
            }
        }
    }
}

void chunk_draw(chunk *chunk) {
    for (int z = 0; z < 16; z++) {
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {
                block_draw(&chunk->blocks[z][y][x]);
            }
        }
    }
}
