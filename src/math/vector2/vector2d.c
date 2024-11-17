#include "vector2d.h"

#include "math.h"

void vector2d_init(vector2d *vector, double x, double y) {
    vector->x = x;
    vector->y = y;
}

void vector2d_normalise(vector2d *vector) {
    double magnitude = sqrt(pow(vector->x, 2) + pow(vector->y, 2));
    if (magnitude == 0) {
        vector2d_init(vector, 0.0, 0.0);
    } else {
        vector->x /= magnitude;
        vector->y /= magnitude;
    }
}

vector2d vector2d_normalised(vector2d vector) {
    vector2d result;
    vector2d_init(&result, 0.0, 0.0);

    double magnitude = sqrt(pow(vector.x, 2) + pow(vector.y, 2));

    if (magnitude != 0) {
        result.x = vector.x / magnitude;
        result.y = vector.y / magnitude;
    }

    return result;
}

void vector2d_to_glm(vector2d vector, vec2 *destination) {
    (*destination)[0] = vector.x;
    (*destination)[1] = vector.y;
}

vector2d vector2d_add(vector2d v1, vector2d v2) {
    vector2d result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;

    return result;
}

vector2d vector2d_sub(vector2d v1, vector2d v2) {
    vector2d result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;

    return result;
}

vector2d vector2d_scalar_multiply(vector2d vector, double scalar) {
    vector2d result;

    result.x = vector.x * scalar;
    result.y = vector.y * scalar;

    return result;
}
