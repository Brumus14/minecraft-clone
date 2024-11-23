#include "matrix4d.h"

void matrix4d_to_glm(matrix4d *matrix, mat4 destination) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            destination[y][x] = matrix->values[y][x];
        }
    }
}

matrix4d matrix4d_from_glm(mat4 glm_matrix) {
    matrix4d matrix;

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            matrix.values[y][x] = glm_matrix[y][x];
        }
    }

    return matrix;
}
