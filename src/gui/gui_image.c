#include "gui_image.h"

// paramter for texture filter
void gui_image_init(gui_image *gui_image, char *image_path, vector2d position,
                    vector2d scale) {
    gui_image->position = position;
    gui_image->scale = scale;

    texture_init(&gui_image->texture, TEXTURE_FILTER_NEAREST,
                 TEXTURE_WRAP_BORDER);
    texture_load(&gui_image->texture, image_path);

    vao_init(&gui_image->vao);
    bo_init(&gui_image->vbo, BO_TYPE_VERTEX);
    bo_init(&gui_image->ibo, BO_TYPE_INDEX);

    vao_bind(&gui_image->vao);
    bo_bind(&gui_image->vbo);
    bo_bind(&gui_image->ibo);

    float vertices[4][5] = {
        {-0.5, -0.5, 0.0, 0.0, 1.0},
        {0.5,  -0.5, 0.0, 1.0, 1.0},
        {0.5,  0.5,  0.0, 1.0, 0.0},
        {-0.5, 0.5,  0.0, 0.0, 0.0},
    };

    for (int i = 0; i < 4; i++) {
        vertices[i][0] *= gui_image->scale.x;
        vertices[i][1] *= gui_image->scale.y;

        vertices[i][0] += gui_image->position.x;
        vertices[i][1] += gui_image->position.y;
    }

    unsigned int indices[6] = {0, 1, 2, 0, 2, 3};

    bo_upload(&gui_image->vbo, 4 * 5 * sizeof(float), vertices,
              BO_USAGE_STATIC_DRAW);

    bo_upload(&gui_image->ibo, 6 * sizeof(unsigned int), indices,
              BO_USAGE_STATIC_DRAW);

    vao_attrib(&gui_image->vao, 0, 3, VAO_TYPE_FLOAT, false, 5 * sizeof(float),
               0);
    vao_attrib(&gui_image->vao, 1, 2, VAO_TYPE_FLOAT, false, 5 * sizeof(float),
               (void *)(3 * sizeof(float)));
}

void gui_image_draw(gui_image *gui_image) {
    texture_bind(&gui_image->texture);
    vao_bind(&gui_image->vao);
    bo_bind(&gui_image->ibo);
    bo_bind(&gui_image->vbo);

    int index_count = bo_get_size(&gui_image->ibo) /
                      sizeof(unsigned int); // already know this value

    renderer_draw_elements(DRAW_MODE_TRIANGLES, index_count,
                           INDEX_TYPE_UNSIGNED_INT);
}
