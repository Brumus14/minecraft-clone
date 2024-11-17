#include "vector2f.h"

#include "math.h"

void vector2f_init(vector2f *vector, float x, float y) {
    vector->x = x;
    vector->y = y;
}

void vector2f_normalise(vector2f *vector) {
    float magnitude = sqrt(pow(vector->x, 2) + pow(vector->y, 2));
    if (magnitude == 0) {
        vector2f_init(vector, 0.0, 0.0);
    } else {
        vector->x /= magnitude;
        vector->y /= magnitude;
    }
}

vector2f vector2f_normalised(vector2f vector) {
    vector2f result;
    vector2f_init(&result, 0.0, 0.0);

    float magnitude = sqrt(pow(vector.x, 2) + pow(vector.y, 2));

    if (magnitude != 0) {
        result.x = vector.x / magnitude;
        result.y = vector.y / magnitude;
    }

    return result;
}

void vector2f_to_glm(vector2f vector, vec2 *destination) {
    (*destination)[0] = vector.x;
    (*destination)[1] = vector.y;
}

vector2f vector2f_add(vector2f v1, vector2f v2) {
    vector2f result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;

    return result;
}

vector2f vector2f_sub(vector2f v1, vector2f v2) {
    vector2f result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;

    return result;
}

vector2f vector2f_scalar_multiply(vector2f vector, float scalar) {
    vector2f result;

    result.x = vector.x * scalar;
    result.y = vector.y * scalar;

    return result;
}
