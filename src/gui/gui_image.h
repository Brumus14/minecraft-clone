#ifndef IMAGE_H
#define IMAGE_H

#include "../graphics/graphics.h"
#include "../math/vector2.h"

typedef struct gui_image {
    vector2d position;
    vector2d scale;
    texture texture;
    vao vao;
    bo vbo;
    bo ibo;
} gui_image;

void gui_image_init(gui_image *gui_image, char *image_path, vector2d position,
                    vector2d scale);
void gui_image_draw(gui_image *gui_image);

#endif
