#include "vector3d.h"

#include "math.h"

void vector3d_init(vector3d *vector, double x, double y, double z) {
    vector->x = x;
    vector->y = y;
    vector->z = z;
}

void vector3d_normalise(vector3d *vector) {
    double magnitude =
        sqrt(pow(vector->x, 2) + pow(vector->y, 2) + pow(vector->z, 2));

    if (magnitude > EPSILON) {
        vector->x /= magnitude;
        vector->y /= magnitude;
        vector->z /= magnitude;
    }
}

vector3d vector3d_normalised(vector3d vector) {
    vector3d result = VECTOR3D_ZERO;

    double magnitude =
        sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));

    if (magnitude > EPSILON) {
        result.x = vector.x / magnitude;
        result.y = vector.y / magnitude;
        result.z = vector.z / magnitude;
    }

    return result;
}

void vector3d_to_glm(vector3d vector, vec3 *destination) {
    (*destination)[0] = vector.x;
    (*destination)[1] = vector.y;
    (*destination)[2] = vector.z;
}

void vector3d_add_to(vector3d v1, vector3d v2, vector3d *dest) {
    dest->x = v1.x + v2.x;
    dest->y = v1.y + v2.y;
    dest->z = v1.z + v2.z;
}

vector3d vector3d_add(vector3d v1, vector3d v2) {
    vector3d result;

    vector3d_add_to(v1, v2, &result);

    return result;
}

vector3d vector3d_sub(vector3d v1, vector3d v2) {
    vector3d result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}

void vector3d_scalar_multiply_to(vector3d vector, double scalar,
                                 vector3d *dest) {
    dest->x = vector.x * scalar;
    dest->y = vector.y * scalar;
    dest->z = vector.z * scalar;
}

vector3d vector3d_scalar_multiply(vector3d vector, double scalar) {
    vector3d result;

    vector3d_scalar_multiply_to(vector, scalar, &result);

    return result;
}

vector3d vector3d_cross_product(vector3d v1, vector3d v2) {
    vector3d result;

    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;

    return result;
}

bool vector3d_equal(vector3d v1, vector3d v2) {
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z) {
        return true;
    } else {
        return false;
    }
}
