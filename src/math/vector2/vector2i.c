#include "vector2i.h"

#include "math.h"

void vector2i_init(vector2i *vector, int x, int y) {
    vector->x = x;
    vector->y = y;
}

void vector2i_to_glm(vector2i vector, vec2 *destination) {
    (*destination)[0] = vector.x;
    (*destination)[1] = vector.y;
}

vector2i vector2i_add(vector2i v1, vector2i v2) {
    vector2i result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;

    return result;
}

vector2i vector2i_sub(vector2i v1, vector2i v2) {
    vector2i result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;

    return result;
}

vector2i vector2i_scalar_multiply(vector2i vector, int scalar) {
    vector2i result;

    result.x = vector.x * scalar;
    result.y = vector.y * scalar;

    return result;
}
