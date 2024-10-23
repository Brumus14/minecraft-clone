#ifndef VECTOR3_H
#define VECTOR3_H

#include "cglm/cglm.h"

typedef struct vector3 {
    float x;
    float y;
    float z;
} vector3;

void vector3_init(vector3 *vector, float x, float y, float z);
void vector3_normalise(vector3 *vector);
vector3 vector3_normalised(vector3 vector);
void vector3_to_glm(vector3 vector, vec3 *destination);
vector3 vector3_add(vector3 v1, vector3 v2);
vector3 vector3_scalar_multiply(vector3 vector, float scalar);
vector3 vector3_cross_product(vector3 v1, vector3 v2);

#endif
