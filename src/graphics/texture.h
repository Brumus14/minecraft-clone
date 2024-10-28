#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"

typedef enum texture_filter {
    TEXTURE_FILTER_LINEAR,
    TEXTURE_FILTER_NEAREST,
} texture_filter;

typedef struct texture {
    GLuint gl_id;
    texture_filter filter;
} texture;

void texture_init(texture *texture, texture_filter filter);
void texture_bind(texture *texture);
void texture_load(texture *texture, char *path);

#endif
