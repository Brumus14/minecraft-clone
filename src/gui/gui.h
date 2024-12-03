#ifndef GUI_H
#define GUI_H

#include "../graphics/window.h"
#include "../graphics/shader_program.h"
#include "gui_element.h"

typedef struct gui {
    gui_element *elements;
    int element_count;
    window *window;
    shader_program shader_program;
    int gl_orthographic_matrix_location;
} gui;

void gui_init(gui *gui, window *window);
void gui_draw(gui *gui);
void gui_add_image(gui *gui, gui_image *image);

#endif
