#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

typedef struct shader {
    GLuint shader_id;
} shader;

typedef enum shader_type {
    SHADER_TYPE_VERTEX,
    SHADER_TYPE_FRAGMENT
} shader_type;

void init_shader(shader *shader, shader_type type);
void source_shader(shader *shader, char *source);
void compile_shader(shader *shader);
void delete_shader(shader *shader);

#endif
