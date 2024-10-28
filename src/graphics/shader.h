#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

typedef struct shader {
    GLuint gl_id;
} shader;

typedef enum shader_type {
    SHADER_TYPE_VERTEX,
    SHADER_TYPE_FRAGMENT
} shader_type;

void shader_init(shader *shader, shader_type type);
void shader_source(shader *shader, char *source);
void shader_compile(shader *shader);
void shader_delete(shader *shader);

#endif
