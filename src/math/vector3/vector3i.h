#ifndef VECTOR3I_H
#define VECTOR3I_H

#include "cglm/cglm.h"

typedef struct vector3i {
    int x;
    int y;
    int z;
} vector3i;

#define VECTOR3I_ZERO                                                          \
    (vector3i) {                                                               \
        0, 0, 0                                                                \
    }

void vector3i_init(vector3i *vector, int x, int y, int z);
void vector3i_normalise(vector3i *vector);
vector3i vector3i_normalised(vector3i vector);
void vector3i_to_glm(vector3i vector, vec3 *destination);
void vector3i_add_to(vector3i v1, vector3i v2, vector3i *dest);
vector3i vector3i_add(vector3i v1, vector3i v2);
vector3i vector3i_sub(vector3i v1, vector3i v2);
void vector3i_scalar_multiply_to(vector3i vector, int scalar, vector3i *dest);
vector3i vector3i_scalar_multiply(vector3i vector, int scalar);
vector3i vector3i_cross_product(vector3i v1, vector3i v2);
bool vector3i_equal(vector3i v1, vector3i v2);

#endif
