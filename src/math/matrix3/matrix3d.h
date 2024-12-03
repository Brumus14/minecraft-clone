#ifndef MATRIX4D_H
#define MATRIX4D_H

#include "cglm/mat3.h"

#define MATRIX4D_ZERO                                                          \
    (matrix3d) {                                                               \
        {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},                     \
    }

typedef struct matrix3d {
    double values[3][3];
} matrix3d;

void matrix3d_to_glm(matrix3d *matrix, mat3 destination);
matrix3d matrix3d_from_glm(mat3 glm_matrix);

#endif
