#ifndef VECTOR3_H
#define VECTOR3_H

#include "cglm/cglm.h"

#define EPSILON 1e-6

typedef struct vector3 {
    float x;
    float y;
    float z;
} vector3;

static const vector3 VECTOR3_ZERO = {0.0, 0.0, 0.0};

void vector3_init(vector3 *vector, float x, float y, float z);
void vector3_normalise(vector3 *vector);
vector3 vector3_normalised(vector3 vector);
void vector3_to_glm(vector3 vector, vec3 *destination);
void vector3_add_to(vector3 v1, vector3 v2, vector3 *dest);
vector3 vector3_add(vector3 v1, vector3 v2);
vector3 vector3_sub(vector3 v1, vector3 v2);
void vector3_scalar_multiply_to(vector3 vector, float scalar, vector3 *dest);
vector3 vector3_scalar_multiply(vector3 vector, float scalar);
vector3 vector3_cross_product(vector3 v1, vector3 v2);
vector3 vector3_add_with_rotation(vector3 vector, vector3 relative_delta,
                                  vector3 rotation);

#endif
