#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct vao {
    GLuint gl_id;
} vao;

typedef enum array_type {
    VAO_TYPE_FLOAT,
    VAO_TYPE_INT,
    VAO_TYPE_UINT
} array_type;

void vao_init(vao *vao);
void vao_bind(vao *vao);
void vao_unbind();
void vao_attrib(vao *vao, int index, int size, array_type type, bool normalised,
                size_t stride, void *pointer);

#endif
