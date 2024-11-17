#ifndef VECTOR2I_H
#define VECTOR2I_H

#include "cglm/cglm.h"

typedef struct vector2i {
    int x;
    int y;
} vector2i;

#define VECTOR2I_ZERO                                                          \
    (vector2i) {                                                               \
        0, 0                                                                   \
    }

void vector2i_init(vector2i *vector, int x, int y);
void vector2i_to_glm(vector2i vector, vec2 *destination);
vector2i vector2i_add(vector2i v1, vector2i v2);
vector2i vector2i_sub(vector2i v1, vector2i v2);
vector2i vector2i_scalar_multiply(vector2i vector, int scalar);

#endif
