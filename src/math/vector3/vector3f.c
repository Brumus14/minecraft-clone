#include "vector3f.h"

#include "math.h"

void vector3f_init(vector3f *vector, float x, float y, float z) {
    vector->x = x;
    vector->y = y;
    vector->z = z;
}

void vector3f_normalise(vector3f *vector) {
    float magnitude =
        sqrt(pow(vector->x, 2) + pow(vector->y, 2) + pow(vector->z, 2));

    if (magnitude > EPSILON) {
        vector->x /= magnitude;
        vector->y /= magnitude;
        vector->z /= magnitude;
    }
}

vector3f vector3f_normalised(vector3f vector) {
    vector3f result = VECTOR3F_ZERO;

    float magnitude =
        sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));

    if (magnitude > EPSILON) {
        result.x = vector.x / magnitude;
        result.y = vector.y / magnitude;
        result.z = vector.z / magnitude;
    }

    return result;
}

void vector3f_to_glm(vector3f vector, vec3 *destination) {
    (*destination)[0] = vector.x;
    (*destination)[1] = vector.y;
    (*destination)[2] = vector.z;
}

void vector3f_add_to(vector3f v1, vector3f v2, vector3f *dest) {
    dest->x = v1.x + v2.x;
    dest->y = v1.y + v2.y;
    dest->z = v1.z + v2.z;
}

vector3f vector3f_add(vector3f v1, vector3f v2) {
    vector3f result;

    vector3f_add_to(v1, v2, &result);

    return result;
}

vector3f vector3f_sub(vector3f v1, vector3f v2) {
    vector3f result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}

void vector3f_scalar_multiply_to(vector3f vector, float scalar,
                                 vector3f *dest) {
    dest->x = vector.x * scalar;
    dest->y = vector.y * scalar;
    dest->z = vector.z * scalar;
}

vector3f vector3f_scalar_multiply(vector3f vector, float scalar) {
    vector3f result;

    vector3f_scalar_multiply_to(vector, scalar, &result);

    return result;
}

vector3f vector3f_cross_product(vector3f v1, vector3f v2) {
    vector3f result;

    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;

    return result;
}

bool vector3f_equal(vector3f v1, vector3f v2) {
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z) {
        return true;
    } else {
        return false;
    }
}

vector3d vector3f_to_vector3d(vector3f vector) {
    return (vector3d){vector.x, vector.y, vector.z};
}
