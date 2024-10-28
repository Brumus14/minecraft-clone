#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>

typedef enum draw_mode { DRAW_MODE_TRIANGLES } draw_mode;

void renderer_init();
void renderer_clear_buffers();
void renderer_set_clear_colour(float red, float green, float blue, float alpha);
void renderer_set_viewport(int x, int y, int width, int height);
void renderer_draw_arrays(draw_mode mode, int first, int count);

#endif
