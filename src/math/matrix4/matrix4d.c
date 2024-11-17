#include "matrix4d.h"

void matrix4d_to_glm(matrix4d *matrix, mat4 destination) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            destination[y][x] = matrix->values[y][x];
        }
    }
}
