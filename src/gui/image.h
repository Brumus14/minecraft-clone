#ifndef GUI_IMAGE_H
#define GUI_IMAGE_H

#define GUI_IMAGE_INDEX_COUNT 6

#include "../graphics/texture.h"
#include "../graphics/vao.h"
#include "../graphics/bo.h"
#include "../math/rectangle.h"
#include "gui_element.h"

typedef struct gui_image {
    bool visible;
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

void gui_image_init(gui_image *gui_image, char *image_path, vector2d position,
                    vector2d scale, gui_element_origin origin,
                    gui_element_layer layer);
void gui_image_draw(gui_image *gui_image);
void gui_image_destroy(gui_image *gui_image);

void gui_image_set_visible(gui_image *gui_image, bool visible);
void gui_image_set_position(gui_image *gui_image, vector2d position);
void gui_image_set_scale(gui_image *gui_image, vector2d scale);
void gui_image_set_texture_rectangle(gui_image *gui_image,
                                     rectangle texture_rectangle);
void gui_image_set_origin(gui_image *gui_image, gui_element_origin origin);
void gui_image_set_layer(gui_image *gui_image, gui_element_layer layer);

#endif
