#include "block.h"

block_texture block_type_to_texture(block_type type) {
    switch (type) {
    case BLOCK_TYPE_GRASS:
        return (block_texture){
            {0, 0, 0, 0, 0, 0}
        };
    }
}

void generate_vertices(int type_index, int texture_tile_columns,
                       int texture_tile_rows) {
}

void block_init(block *block, vector3 position, block_type type,
                tilemap tilemap) {
    rectangle tile_rect = tilemap_get_tile_rectangle(&tilemap, 1);
    float tile_rect_right = tile_rect.x + tile_rect.width;
    float tile_rect_bottom = tile_rect.y + tile_rect.height;

    float vertices[][5] = {
        // front
        {0.0, -1.0, 1.0, tile_rect.x,     tile_rect_bottom},
        {1.0, -1.0, 1.0, tile_rect_right, tile_rect_bottom},
        {1.0, 0.0,  1.0, tile_rect_right, tile_rect.y     },
        {0.0, 0.0,  1.0, tile_rect.x,     tile_rect.y     },

        // top
        {0.0, 0.0,  1.0, tile_rect.x,     tile_rect_bottom},
        {1.0, 0.0,  1.0, tile_rect_right, tile_rect_bottom},
        {1.0, 0.0,  0.0, tile_rect_right, tile_rect.y     },
        {0.0, 0.0,  0.0, tile_rect.x,     tile_rect.y     },

        // right
        {1.0, -1.0, 1.0, tile_rect.x,     tile_rect_bottom},
        {1.0, -1.0, 0.0, tile_rect_right, tile_rect_bottom},
        {1.0, 0.0,  0.0, tile_rect_right, tile_rect.y     },
        {1.0, 0.0,  1.0, tile_rect.x,     tile_rect.y     },

        // bottom
        {0.0, -1.0, 0.0, tile_rect.x,     tile_rect_bottom},
        {1.0, -1.0, 0.0, tile_rect_right, tile_rect_bottom},
        {1.0, -1.0, 1.0, tile_rect_right, tile_rect.y     },
        {0.0, -1.0, 1.0, tile_rect.x,     tile_rect.y     },

        // left
        {0.0, -1.0, 0.0, tile_rect.x,     tile_rect_bottom},
        {0.0, -1.0, 1.0, tile_rect_right, tile_rect_bottom},
        {0.0, 0.0,  1.0, tile_rect_right, tile_rect.y     },
        {0.0, 0.0,  0.0, tile_rect.x,     tile_rect.y     },

        // back
        {1.0, -1.0, 0.0, tile_rect.x,     tile_rect_bottom},
        {0.0, -1.0, 0.0, tile_rect_right, tile_rect_bottom},
        {0.0, 0.0,  0.0, tile_rect_right, tile_rect.y     },
        {1.0, 0.0,  0.0, tile_rect.x,     tile_rect.y     },
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
    glActiveTexture(GL_TEXTURE0);
    tilemap_bind(&block->tilemap);
    bo_bind(&block->vbo);
    bo_bind(&block->ibo);
    vao_bind(&block->vao);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
