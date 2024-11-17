#include "vector3i.h"

#include "math.h"

void vector3i_init(vector3i *vector, int x, int y, int z) {
    vector->x = x;
    vector->y = y;
    vector->z = z;
}

void vector3i_add_to(vector3i v1, vector3i v2, vector3i *dest) {
    dest->x = v1.x + v2.x;
    dest->y = v1.y + v2.y;
    dest->z = v1.z + v2.z;
}

vector3i vector3i_add(vector3i v1, vector3i v2) {
    vector3i result;

    vector3i_add_to(v1, v2, &result);

    return result;
}

vector3i vector3i_sub(vector3i v1, vector3i v2) {
    vector3i result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}

void vector3i_scalar_multiply_to(vector3i vector, int scalar, vector3i *dest) {
    dest->x = vector.x * scalar;
    dest->y = vector.y * scalar;
    dest->z = vector.z * scalar;
}

vector3i vector3i_scalar_multiply(vector3i vector, int scalar) {
    vector3i result;

    vector3i_scalar_multiply_to(vector, scalar, &result);

    return result;
}

bool vector3i_equal(vector3i v1, vector3i v2) {
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z) {
        return true;
    } else {
        return false;
    }
}
