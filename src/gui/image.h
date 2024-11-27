#ifndef GUI_IMAGE_H
#define GUI_IMAGE_H

#include "../graphics/graphics.h"
#include "../math/vector2.h"
#include "../math/rectangle.h"
#include "gui_element.h"

typedef struct gui_image {
    vector2d position;
    vector2d scale;
    texture texture;
    rectangle texture_rectangle;
    gui_element_origin origin;
    gui_element_layer layer;
    vao vao;
    bo vbo;
    bo ibo;
} gui_image;

void gui_image_init(gui_image *gui_image, char *image_path,
                    rectangle texture_rectangle, vector2d position,
                    vector2d scale, gui_element_origin origin,
                    gui_element_layer layer);
void gui_image_draw(gui_image *gui_image);
void gui_image_set_scale(gui_image *gui_image, vector2d scale);
void gui_image_set_position(gui_image *gui_image, vector2d position);

#endif
