#ifndef VECTOR3F_H
#define VECTOR3F_H

#include "cglm/cglm.h"
#include "vector3d.h"

#define EPSILON 1e-6

typedef struct vector3f {
    float x;
    float y;
    float z;
} vector3f;

#define VECTOR3F_ZERO                                                          \
    (vector3f) {                                                               \
        0.0, 0.0, 0.0                                                          \
    }

void vector3f_init(vector3f *vector, float x, float y, float z);
void vector3f_normalise(vector3f *vector);
vector3f vector3f_normalised(vector3f vector);
void vector3f_to_glm(vector3f vector, vec3 *destination);
void vector3f_add_to(vector3f v1, vector3f v2, vector3f *dest);
vector3f vector3f_add(vector3f v1, vector3f v2);
vector3f vector3f_sub(vector3f v1, vector3f v2);
void vector3f_scalar_multiply_to(vector3f vector, float scalar, vector3f *dest);
vector3f vector3f_scalar_multiply(vector3f vector, float scalar);
vector3f vector3f_cross_product(vector3f v1, vector3f v2);
bool vector3f_equal(vector3f v1, vector3f v2);

vector3d vector3f_to_vector3d(vector3f vector);

#endif
