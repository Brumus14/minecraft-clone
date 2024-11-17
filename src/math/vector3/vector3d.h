#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "cglm/cglm.h"

#define EPSILON 1e-6

typedef struct vector3d {
    double x;
    double y;
    double z;
} vector3d;

#define VECTOR3D_ZERO                                                          \
    (vector3d) {                                                               \
        0.0, 0.0, 0.0                                                          \
    }

void vector3d_init(vector3d *vector, double x, double y, double z);
void vector3d_normalise(vector3d *vector);
vector3d vector3d_normalised(vector3d vector);
void vector3d_to_glm(vector3d vector, vec3 *destination);
void vector3d_add_to(vector3d v1, vector3d v2, vector3d *dest);
vector3d vector3d_add(vector3d v1, vector3d v2);
vector3d vector3d_sub(vector3d v1, vector3d v2);
void vector3d_scalar_multiply_to(vector3d vector, double scalar,
                                 vector3d *dest);
vector3d vector3d_scalar_multiply(vector3d vector, double scalar);
vector3d vector3d_cross_product(vector3d v1, vector3d v2);
bool vector3d_equal(vector3d v1, vector3d v2);

#endif
