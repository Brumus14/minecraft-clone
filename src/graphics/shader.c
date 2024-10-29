#include "shader.h"
#include <stdio.h>

#include "gl_util.h"

GLenum to_shader_type_gl(shader_type type) {
    switch (type) {
    case SHADER_TYPE_VERTEX:
        return GL_VERTEX_SHADER;
    case SHADER_TYPE_FRAGMENT:
        return GL_FRAGMENT_SHADER;
    }
}

void shader_init(shader *shader, shader_type type) {
    GLenum gl_type = to_shader_type_gl(type);

    shader->gl_id = GL_CALL_R(glCreateShader(gl_type), GLuint);
}

void shader_source(shader *shader, char *source) {
    const GLchar *sources[] = {source};
    GL_CALL(glShaderSource(shader->gl_id, 1, sources, 0));
}

void shader_compile(shader *shader) {
    GL_CALL(glCompileShader(shader->gl_id));

    GLint val = GL_FALSE;
    GL_CALL(glGetShaderiv(shader->gl_id, GL_COMPILE_STATUS, &val));
}

void shader_delete(shader *shader) {
    GL_CALL(glDeleteShader(shader->gl_id));
}
