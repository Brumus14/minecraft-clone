#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "shader.h"

typedef struct shader_program {
    GLuint shader_program_id;
} shader_program;

void init_shader_program(shader_program *program, shader *vertex_shader,
                         shader *fragment_shader);
void bind_attribute_shader_program(shader_program *program, int index,
                                   char *name);
void link_shader_program(shader_program *program);
void use_shader_program(shader_program *program);

#endif
