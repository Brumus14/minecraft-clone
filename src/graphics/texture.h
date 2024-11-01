#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"

typedef enum texture_filter {
    TEXTURE_FILTER_LINEAR,
    TEXTURE_FILTER_NEAREST,
} texture_filter;

typedef enum texture_wrap {
    TEXTURE_WRAP_BORDER,
    TEXTURE_WRAP_REPEAT,
} texture_wrap;

typedef struct texture {
    GLuint gl_id;
    texture_filter filter;
    texture_wrap wrap;
} texture;

void texture_init(texture *texture, texture_filter filter, texture_wrap wrap);
void texture_bind(texture *texture);
void texture_load(texture *texture, char *path);

#endif
