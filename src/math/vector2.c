#include "vector2.h"

#include "math.h"

void vector2_init(vector2 *vector, float x, float y) {
    vector->x = x;
    vector->y = y;
}

void vector2_normalise(vector2 *vector) {
    float magnitude = sqrt(pow(vector->x, 2) + pow(vector->y, 2));

    if (magnitude == 0) {
        vector2_init(vector, 0.0, 0.0);
    } else {
        vector->x /= magnitude;
        vector->y /= magnitude;
    }
}

vector2 vector2_normalised(vector2 vector) {
    vector2 result;
    vector2_init(&result, 0.0, 0.0);

    float magnitude = sqrt(pow(vector.x, 2) + pow(vector.y, 2));

    if (magnitude != 0) {
        result.x = vector.x / magnitude;
        result.y = vector.y / magnitude;
    }

    return result;
}

void vector2_to_glm(vector2 vector, vec2 *destination) {
    (*destination)[0] = vector.x;
    (*destination)[1] = vector.y;
}

vector2 vector2_add(vector2 v1, vector2 v2) {
    vector2 result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;

    return result;
}

vector2 vector2_sub(vector2 v1, vector2 v2) {
    vector2 result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;

    return result;
}

vector2 vector2_scalar_multiply(vector2 vector, float scalar) {
    vector2 result;

    result.x = vector.x * scalar;
    result.y = vector.y * scalar;

    return result;
}
