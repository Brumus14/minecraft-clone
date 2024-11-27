#ifndef GUI_H
#define GUI_H

#include "../graphics/graphics.h"
#include "../math/matrix4/matrix4d.h"
#include "gui_element.h"

typedef struct gui {
    gui_element *elements;
    int element_count;
    window *window;
    shader_program shader_program;
    matrix4d orthographic_matrix;
} gui;

void gui_init(gui *gui, window *window);
void gui_draw(gui *gui);
void gui_add_image(gui *gui, gui_image *image);

#endif
