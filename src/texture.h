#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"

typedef struct texture {
    GLuint gl_id;
} texture;

void texture_init(texture *texture);
void texture_bind(texture *texture);
void texture_load(texture *texture, char *path);

#endif
