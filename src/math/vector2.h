#ifndef VECTOR2_H
#define VECTOR2_H

#include "cglm/cglm.h"

typedef struct vector2 {
    float x;
    float y;
} vector2;

static const vector2 VECTOR2_ZERO = {0.0, 0.0};

void vector2_init(vector2 *vector, float x, float y);
void vector2_normalise(vector2 *vector);
vector2 vector2_normalised(vector2 vector);
void vector2_to_glm(vector2 vector, vec2 *destination);
vector2 vector2_add(vector2 v1, vector2 v2);
vector2 vector2_sub(vector2 v1, vector2 v2);
vector2 vector2_scalar_multiply(vector2 vector, float scalar);

#endif
