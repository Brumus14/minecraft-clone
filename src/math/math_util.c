#include "math_util.h"

vector3d rotation_to_direction(vector3d rotation) {
    vector3d direction;
    rotation.y -= 90;

    direction.x = cos(glm_rad(rotation.y)) * cos(glm_rad(rotation.x));
    direction.y = sin(glm_rad(rotation.x));
    direction.z = sin(glm_rad(rotation.y)) * cos(glm_rad(rotation.x));

    vector3d_normalise(&direction);

    return direction;
}

float clamp(float value, float min, float max) {
    float min_value = value < min ? min : value;
    return min_value > max ? max : min_value;
}

void vector3f_print(vector3f v) {
    printf("%f, %f, %f\n", v.x, v.y, v.z);
}

void vector3d_print(vector3d v) {
    printf("%f, %f, %f\n", v.x, v.y, v.z);
}

void vector3i_print(vector3i v) {
    printf("%d, %d, %d\n", v.x, v.y, v.z);
}

int sign(float value) {
    if (value == 0) {
        return 0;
    }

    return value / fabsf(value);
}

int mod(int a, int b) {
    int r = a % b;

    return r < 0 ? r + b : r;
}
