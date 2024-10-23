#include "vao.h"

void init_vao(vao *vao) {
    glGenVertexArrays(1, &vao->vao_id);
}

void bind_vao(vao *vao) {
    glBindVertexArray(vao->vao_id);
}

GLenum to_gl_array_type(array_type type) {
    switch (type) {
    case FLOAT:
        return GL_FLOAT;

    case INT:
        return GL_INT;

    case UINT:
        return GL_UNSIGNED_INT;
    }
}

void attrib_vao(vao *vao, int index, int size, array_type type, bool normalised,
                size_t stride, void *pointer) {
    GLenum gl_type = to_gl_array_type(type);

    glVertexAttribPointer(index, size, gl_type, normalised, stride, pointer);
    glEnableVertexAttribArray(0);
}
