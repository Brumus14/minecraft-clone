#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include "vector3/vector3f.h"

vector3d rotation_to_direction(vector3d rotation);
float clamp(float value, float min, float max);
void vector3f_print(vector3f v);
int sign(float value);

#endif
