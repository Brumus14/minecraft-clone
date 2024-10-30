#include "vao.h"

#include <stdio.h>
#include "gl_util.h"

void vao_init(vao *vao) {
    GL_CALL(glGenVertexArrays(1, &vao->gl_id));
}

void vao_bind(vao *vao) {
    GL_CALL(glBindVertexArray(vao->gl_id));
}

void vao_unbind() {
    GL_CALL(glBindVertexArray(0));
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

    GL_CALL(glVertexAttribPointer(index, size, gl_type, normalised, stride,
                                  pointer));
    GL_CALL(glEnableVertexAttribArray(index));
}
