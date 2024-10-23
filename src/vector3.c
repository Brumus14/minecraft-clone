#include "vector3.h"

#include "math.h"

void vector3_init(vector3 *vector, float x, float y, float z) {
    vector->x = x;
    vector->y = y;
    vector->z = z;
}

void vector3_normalise(vector3 *vector) {
    float magnitude =
        sqrt(pow(vector->x, 2) + pow(vector->y, 2) + pow(vector->z, 2));

    if (magnitude == 0) {
        vector3_init(vector, 0.0, 0.0, 0.0);
    } else {
        vector->x /= magnitude;
        vector->y /= magnitude;
        vector->z /= magnitude;
    }
}

vector3 vector3_normalised(vector3 vector) {
    vector3 result;
    vector3_init(&result, 0.0, 0.0, 0.0);

    float magnitude =
        sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));

    if (magnitude != 0) {
        result.x = vector.x / magnitude;
        result.y = vector.y / magnitude;
        result.z = vector.z / magnitude;
    }

    return result;
}

void vector3_to_glm(vector3 vector, vec3 *destination) {
    (*destination)[0] = vector.x;
    (*destination)[1] = vector.y;
    (*destination)[2] = vector.z;
}

vector3 vector3_add(vector3 v1, vector3 v2) {
    vector3 result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;

    return result;
}

vector3 vector3_scalar_multiply(vector3 vector, float scalar) {
    vector3 result;

    result.x = vector.x * scalar;
    result.y = vector.y * scalar;
    result.z = vector.z * scalar;

    return result;
}

vector3 vector3_cross_product(vector3 v1, vector3 v2) {
    vector3 result;

    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;

    return result;
}
