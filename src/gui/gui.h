#ifndef GUI_H
#define GUI_H

#include "../graphics/graphics.h"
#include "../math/matrix4/matrix4d.h"

typedef struct gui {
    shader_program shader_program;
    matrix4d orthographic_matrix;
    window *window;
} gui;

void gui_init(gui *gui, window *window);
void gui_prepare_draw(gui *gui);

#endif
