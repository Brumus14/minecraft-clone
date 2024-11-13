#include "math_util.h"

vector3 rotation_to_direction(vector3 rotation) {
    vector3 direction;
    rotation.y -= 90;

    direction.x = cos(glm_rad(rotation.y)) * cos(glm_rad(rotation.x));
    direction.y = sin(glm_rad(rotation.x));
    direction.z = sin(glm_rad(rotation.y)) * cos(glm_rad(rotation.x));

    vector3_normalise(&direction);

    return direction;
}

float clamp(float value, float min, float max) {
    float min_value = value < min ? min : value;
    return min_value > max ? max : min_value;
}
