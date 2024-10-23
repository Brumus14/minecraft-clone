#include "shader_program.h"

void init_shader_program(shader_program *program, shader *vertex_shader,
                         shader *fragment_shader) {
    program->shader_program_id = glCreateProgram();
    glAttachShader(program->shader_program_id, vertex_shader->shader_id);
    glAttachShader(program->shader_program_id, fragment_shader->shader_id);
}

void bind_attribute_shader_program(shader_program *program, int index,
                                   char *name) {
    glBindAttribLocation(program->shader_program_id, index, name);
}

void link_shader_program(shader_program *program) {
    glLinkProgram(program->shader_program_id);
}

void use_shader_program(shader_program *program) {
    glUseProgram(program->shader_program_id);
}
