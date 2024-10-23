#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>

typedef enum draw_mode { TRIANGLES } draw_mode;

void init_renderer();
void clear_colour();
void set_clear_colour(float red, float green, float blue, float alpha);
void set_viewport(int x, int y, int width, int height);
GLenum to_gl_draw_mode(draw_mode mode);
void draw_arrays(draw_mode mode, int first, int count);

#endif
