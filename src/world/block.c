#include "block.h"

block_texture block_type_to_texture(block_type type) {
    // front, top, right, bottom, left, back
    switch (type) {
    case BLOCK_TYPE_GRASS:
        return (block_texture){
            {1, 0, 1, 2, 1, 1}
        };
    }
}

void generate_vertices(int type_index, int texture_tile_columns,
                       int texture_tile_rows) {
}

void block_init(block *block, vector3 position, block_type type,
                tilemap *tilemap) {
    block->tilemap = tilemap;

    block_texture texture = block_type_to_texture(type);

    rectangle tile_rects[6];
    float tile_rect_rights[6];
    float tile_rect_bottoms[6];

    for (int i = 0; i < 6; i++) {
        tile_rects[i] =
            tilemap_get_tile_rectangle(tilemap, texture.face_textures[i]);
        tile_rect_rights[i] = tile_rects[i].x + tile_rects[i].width;
        tile_rect_bottoms[i] = tile_rects[i].y + tile_rects[i].height;
    }

    float vertices[][5] = {
        // front
        {0.0, -1.0, 1.0, tile_rects[0].x,     tile_rect_bottoms[0]},
        {1.0, -1.0, 1.0, tile_rect_rights[0], tile_rect_bottoms[0]},
        {1.0, 0.0,  1.0, tile_rect_rights[0], tile_rects[0].y     },
        {0.0, 0.0,  1.0, tile_rects[0].x,     tile_rects[0].y     },

        // top
        {0.0, 0.0,  1.0, tile_rects[1].x,     tile_rect_bottoms[1]},
        {1.0, 0.0,  1.0, tile_rect_rights[1], tile_rect_bottoms[1]},
        {1.0, 0.0,  0.0, tile_rect_rights[1], tile_rects[1].y     },
        {0.0, 0.0,  0.0, tile_rects[1].x,     tile_rects[1].y     },

        // right
        {1.0, -1.0, 1.0, tile_rects[2].x,     tile_rect_bottoms[2]},
        {1.0, -1.0, 0.0, tile_rect_rights[2], tile_rect_bottoms[2]},
        {1.0, 0.0,  0.0, tile_rect_rights[2], tile_rects[2].y     },
        {1.0, 0.0,  1.0, tile_rects[2].x,     tile_rects[2].y     },

        // bottom
        {0.0, -1.0, 0.0, tile_rects[3].x,     tile_rect_bottoms[3]},
        {1.0, -1.0, 0.0, tile_rect_rights[3], tile_rect_bottoms[3]},
        {1.0, -1.0, 1.0, tile_rect_rights[3], tile_rects[3].y     },
        {0.0, -1.0, 1.0, tile_rects[3].x,     tile_rects[3].y     },

        // left
        {0.0, -1.0, 0.0, tile_rects[4].x,     tile_rect_bottoms[4]},
        {0.0, -1.0, 1.0, tile_rect_rights[4], tile_rect_bottoms[4]},
        {0.0, 0.0,  1.0, tile_rect_rights[4], tile_rects[4].y     },
        {0.0, 0.0,  0.0, tile_rects[4].x,     tile_rects[4].y     },

        // back
        {1.0, -1.0, 0.0, tile_rects[5].x,     tile_rect_bottoms[5]},
        {0.0, -1.0, 0.0, tile_rect_rights[5], tile_rect_bottoms[5]},
        {0.0, 0.0,  0.0, tile_rect_rights[5], tile_rects[5].y     },
        {1.0, 0.0,  0.0, tile_rects[5].x,     tile_rects[5].y     },
    };

    unsigned int indices[] = {
        0,  1,  2,  0,  2,  3,  // front
        4,  5,  6,  4,  6,  7,  // top
        8,  9,  10, 8,  10, 11, // right
        12, 13, 14, 12, 14, 15, // bottom
        16, 17, 18, 16, 18, 19, // left
        20, 21, 22, 20, 22, 23, // back
    };

    for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i++) {
        vertices[i][0] += position.x;
        vertices[i][1] += position.y;
        vertices[i][2] += position.z;
    }

    bo_init(&block->vbo, BO_TYPE_VERTEX);
    bo_bind(&block->vbo);
    bo_upload(&block->vbo, sizeof(vertices), vertices, BO_USAGE_STATIC_DRAW);

    bo_init(&block->ibo, BO_TYPE_INDEX);
    bo_bind(&block->ibo);
    bo_upload(&block->ibo, sizeof(indices), indices, BO_USAGE_STATIC_DRAW);

    vao_init(&block->vao);
    vao_bind(&block->vao);
    vao_attrib(&block->vao, 0, 3, VAO_TYPE_FLOAT, false, 5 * sizeof(float), 0);
    vao_attrib(&block->vao, 1, 2, VAO_TYPE_FLOAT, false, 5 * sizeof(float),
               (void *)(3 * sizeof(float)));
}

void block_draw(block *block) {
    tilemap_bind(block->tilemap);
    bo_bind(&block->vbo);
    bo_bind(&block->ibo);
    vao_bind(&block->vao);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
