#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct vao {
    GLuint vao_id;
} vao;

typedef enum array_type { FLOAT, INT, UINT } array_type;

void init_vao(vao *vao);
void bind_vao(vao *vao);
void attrib_vao(vao *vao, int index, int size, array_type type, bool normalised,
                size_t stride, void *pointer);

#endif
