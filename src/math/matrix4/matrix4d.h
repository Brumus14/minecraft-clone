#ifndef MATRIX4D_H
#define MATRIX4D_H

#include "cglm/mat4.h"

#define MATRIX4D_ZERO                                                          \
    (matrix4d) {                                                               \
        {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0},      \
            {0.0, 0.0, 0.0, 0.0},                                              \
    }

typedef struct matrix4d {
    double values[4][4];
} matrix4d;

void matrix4d_to_glm(matrix4d *matrix, mat4 destination);
matrix4d matrix4d_from_glm(mat4 glm_matrix);

#endif
