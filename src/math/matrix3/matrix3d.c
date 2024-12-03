#include "matrix3d.h"

void matrix3d_to_glm(matrix3d *matrix, mat3 destination) {
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            destination[y][x] = matrix->values[y][x];
        }
    }
}

matrix3d matrix3d_from_glm(mat3 glm_matrix) {
    matrix3d matrix;

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            matrix.values[y][x] = glm_matrix[y][x];
        }
    }

    return matrix;
}
