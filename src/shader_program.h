#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "shader.h"

typedef struct shader_program {
    GLuint gl_id;
    /*int shader_keys[5];*/
    /*int shader_gl_id_values[5];*/
} shader_program;

void shader_program_init(shader_program *program, shader *vertex_shader,
                         shader *fragment_shader);
void shader_program_from_files(shader_program *program, char *vertex_path,
                               char *fragment_path);
void shader_program_bind_attribute(shader_program *program, int index,
                                   char *name);
void shader_program_link(shader_program *program);
void shader_program_use(shader_program *program);
void shader_program_delete(shader_program *program);
/*void shader_program_delete_all(shader_program *program);*/

#endif
