#ifndef RENDERER_H
#define RENDERER_H

#include "glad/glad.h"

typedef enum draw_mode { DRAW_MODE_TRIANGLES } draw_mode;

typedef enum polygon_mode {
    POLYGON_MODE_FILL,
    POLYGON_MODE_LINE,
} polygon_mode;

typedef enum index_type {
    INDEX_TYPE_UNSIGNED_INT,
    INDEX_TYPE_UNSIGNED_SHORT,
    INDEX_TYPE_UNSIGNED_BYTE,
} index_type;

void renderer_init();
void renderer_clear_buffers();
void renderer_set_clear_colour(float red, float green, float blue, float alpha);
void renderer_set_viewport(int x, int y, int width, int height);
void renderer_draw_elements(draw_mode mode, int count, index_type type);
void renderer_set_polygon_mode(polygon_mode mode);

#endif
