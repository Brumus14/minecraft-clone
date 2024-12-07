#include "shader.h"

#include <stdio.h>
#include "../util/gl.h"

GLenum to_shader_type_gl(shader_type type) {
    switch (type) {
    case SHADER_TYPE_VERTEX:
        return GL_VERTEX_SHADER;
    case SHADER_TYPE_FRAGMENT:
        return GL_FRAGMENT_SHADER;
    }
}

void shader_init(shader *shader, shader_type type) {
    if (shader == NULL) {
        fprintf(stderr, "shader_init: shader is null\n");
        return;
    }

    GLenum gl_type = to_shader_type_gl(type);

    shader->gl_id = GL_CALL_R(glCreateShader(gl_type), GLuint);
}

void shader_source(shader *shader, char *source) {
    if (shader == NULL) {
        fprintf(stderr, "shader_source: shader is null\n");
        return;
    }

    const GLchar *sources[] = {source};
    GL_CALL(glShaderSource(shader->gl_id, 1, sources, 0));
}

void shader_compile(shader *shader) {
    if (shader == NULL) {
        fprintf(stderr, "shader_compile: shader is null\n");
        return;
    }

    GL_CALL(glCompileShader(shader->gl_id));

    GLint compile_status;
    GL_CALL(glGetShaderiv(shader->gl_id, GL_COMPILE_STATUS, &compile_status));

    if (compile_status == GL_FALSE) {
        int max_length = 0;
        GL_CALL(glGetShaderiv(shader->gl_id, GL_INFO_LOG_LENGTH, &max_length));

        char error[max_length];
        glGetShaderInfoLog(shader->gl_id, max_length, &max_length,
                           (GLchar *)&error);

        fprintf(stderr, "shader_compile: %s\n", error);
    }
}

void shader_delete(shader *shader) {
    if (shader == NULL) {
        fprintf(stderr, "shader_delete: shader is null\n");
        return;
    }

    GL_CALL(glDeleteShader(shader->gl_id));
}
