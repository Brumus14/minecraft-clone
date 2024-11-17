#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "cglm/cglm.h"

typedef struct vector2d {
    double x;
    double y;
} vector2d;

#define VECTOR2D_ZERO                                                          \
    (vector2d) {                                                               \
        0.0, 0.0                                                               \
    }

void vector2d_init(vector2d *vector, double x, double y);
void vector2d_normalise(vector2d *vector);
vector2d vector2d_normalised(vector2d vector);
void vector2d_to_glm(vector2d vector, vec2 *destination);
vector2d vector2d_add(vector2d v1, vector2d v2);
vector2d vector2d_sub(vector2d v1, vector2d v2);
vector2d vector2d_scalar_multiply(vector2d vector, double scalar);

#endif
