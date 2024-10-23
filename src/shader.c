#include "shader.h"

GLenum to_shader_type_gl(shader_type type) {
    switch (type) {
    case SHADER_TYPE_VERTEX:
        return GL_VERTEX_SHADER;
    case SHADER_TYPE_FRAGMENT:
        return GL_FRAGMENT_SHADER;
    }
}

void init_shader(shader *shader, shader_type type) {
    GLenum gl_type = to_shader_type_gl(type);

    shader->shader_id = glCreateShader(gl_type);
}

void source_shader(shader *shader, char *source) {
    const GLchar *sources[] = {source};
    glShaderSource(shader->shader_id, 1, sources, 0);
}

void compile_shader(shader *shader) {
    glCompileShader(shader->shader_id);
}

void delete_shader(shader *shader) {
    glDeleteShader(shader->shader_id);
}
