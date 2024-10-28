#include "vao.h"

#include <stdio.h>

void vao_init(vao *vao) {
    glGenVertexArrays(1, &vao->gl_id);
}

void vao_bind(vao *vao) {
    glBindVertexArray(vao->gl_id);
}

GLenum to_gl_array_type(array_type type) {
    switch (type) {
    case VAO_TYPE_FLOAT:
        return GL_FLOAT;
    case VAO_TYPE_INT:
        return GL_INT;
    case VAO_TYPE_UINT:
        return GL_UNSIGNED_INT;
    default:
        fprintf(stderr, "to_gl_array_type: array type wasnt recognised");
        return GL_FLOAT;
    }
}

void vao_attrib(vao *vao, int index, int size, array_type type, bool normalised,
                size_t stride, void *pointer) {
    GLenum gl_type = to_gl_array_type(type);

    glVertexAttribPointer(index, size, gl_type, normalised, stride, pointer);
    glEnableVertexAttribArray(0);
}
