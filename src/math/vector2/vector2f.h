#ifndef VECTOR2F_H
#define VECTOR2F_H

#include "cglm/cglm.h"

typedef struct vector2f {
    float x;
    float y;
} vector2f;

#define VECTOR2F_ZERO                                                          \
    (vector2f) {                                                               \
        0.0, 0.0                                                               \
    }

void vector2f_init(vector2f *vector, float x, float y);
void vector2f_normalise(vector2f *vector);
vector2f vector2f_normalised(vector2f vector);
void vector2f_to_glm(vector2f vector, vec2 *destination);
vector2f vector2f_add(vector2f v1, vector2f v2);
vector2f vector2f_sub(vector2f v1, vector2f v2);
vector2f vector2f_scalar_multiply(vector2f vector, float scalar);

#endif
