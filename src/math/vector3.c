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

    if (magnitude > EPSILON) {
        vector->x /= magnitude;
        vector->y /= magnitude;
        vector->z /= magnitude;
    }
}

vector3 vector3_normalised(vector3 vector) {
    vector3 result = VECTOR3_ZERO;

    float magnitude =
        sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));

    if (magnitude > EPSILON) {
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

void vector3_add_to(vector3 v1, vector3 v2, vector3 *dest) {
    dest->x = v1.x + v2.x;
    dest->y = v1.y + v2.y;
    dest->z = v1.z + v2.z;
}

vector3 vector3_add(vector3 v1, vector3 v2) {
    vector3 result;

    vector3_add_to(v1, v2, &result);

    return result;
}

vector3 vector3_sub(vector3 v1, vector3 v2) {
    vector3 result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}

void vector3_scalar_multiply_to(vector3 vector, float scalar, vector3 *dest) {
    dest->x = vector.x * scalar;
    dest->y = vector.y * scalar;
    dest->z = vector.z * scalar;
}

vector3 vector3_scalar_multiply(vector3 vector, float scalar) {
    vector3 result;

    vector3_scalar_multiply_to(vector, scalar, &result);

    return result;
}

vector3 vector3_cross_product(vector3 v1, vector3 v2) {
    vector3 result;

    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;

    return result;
}

/*vector3 vector3_add_with_rotation(vector3 vector, vector3 relative_delta,*/
/*                                  vector3 rotation) {*/
/*    // Calculate direction from rotation*/
/*    vector3 direction;*/
/**/
/*    direction.x = cos(glm_rad(rotation.y)) * cos(glm_rad(rotation.x));*/
/*    direction.y = sin(glm_rad(rotation.x));*/
/*    direction.z = sin(glm_rad(rotation.y)) * cos(glm_rad(rotation.x));*/
/**/
/*    vector3_normalise(&direction);*/
/**/
/*    vector3 delta;*/
/*    vector3_init(&delta, 0.0, 0.0, 0.0);*/
/**/
/*    // Add forwards delta*/
/*    delta = vector3_add(delta,*/
/*                        vector3_scalar_multiply(direction,
 * -relative_delta.z));*/
/**/
/*    vector3 up;*/
/*    vector3_init(&up, 0.0, 1.0, 0.0);*/
/**/
/*    delta = vector3_add(*/
/*        delta, vector3_scalar_multiply(*/
/*                   vector3_normalised(vector3_cross_product(direction, up)),*/
/*                   relative_delta.x));*/
/**/
/*    delta.y += relative_delta.y;*/
/**/
/*    return vector3_add(vector, delta);*/
/*}*/
