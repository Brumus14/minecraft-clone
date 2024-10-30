#include "shader_program.h"

#include <stdio.h>
#include "gl_util.h"
#include "../util.h"

void shader_program_init(shader_program *program, shader *vertex_shader,
                         shader *fragment_shader) {
    program->gl_id = GL_CALL_R(glCreateProgram(), GLuint);
    GL_CALL(glAttachShader(program->gl_id, vertex_shader->gl_id));
    GL_CALL(glAttachShader(program->gl_id, fragment_shader->gl_id));

    shader_program_link(program);

    GL_CALL(glDeleteShader(vertex_shader->gl_id));
    GL_CALL(glDeleteShader(fragment_shader->gl_id));
}

void shader_program_from_files(shader_program *program, char *vertex_path,
                               char *fragment_path) {
    char *vertex_source = read_file(vertex_path);
    char *fragment_source = read_file(fragment_path);

    if (!vertex_source) {
        fprintf(stderr, "shader_program_from_files: vertex source is null\n");

        return;
    }

    if (!fragment_source) {
        fprintf(stderr, "shader_program_from_files: fragment source is null\n");

        return;
    }

    shader vertex_shader;
    shader_init(&vertex_shader, SHADER_TYPE_VERTEX);
    shader_source(&vertex_shader, vertex_source);
    shader_compile(&vertex_shader);

    shader fragment_shader;
    shader_init(&fragment_shader, SHADER_TYPE_FRAGMENT);
    shader_source(&fragment_shader, fragment_source);
    shader_compile(&fragment_shader);

    shader_program_init(program, &vertex_shader, &fragment_shader);
}

void shader_program_bind_attribute(shader_program *program, int index,
                                   char *name) {

    GL_CALL(glBindAttribLocation(program->gl_id, index, name));
}

void shader_program_link(shader_program *program) {
    GL_CALL(glLinkProgram(program->gl_id));
}

void shader_program_use(shader_program *program) {
    GL_CALL(glUseProgram(program->gl_id));
}

void shader_program_delete(shader_program *program) {
    shader_program_use(program);
    GL_CALL(glDeleteProgram(program->gl_id));
}

/*void shader_program_delete_all(shader_program *program) {*/
/*    for (int i = 0; i < 5; ++i) {*/
/*        if (program->shader_gl_id_values[i] != 0) {*/
/*            glDeleteShader(program->shader_gl_id_values[i]);*/
/*        }*/
/*    }*/
/**/
/*    shader_program_use(program);*/
/*    glDeleteProgram(program->gl_id);*/
/*}*/
