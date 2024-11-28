#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include "vector3/vector3f.h"
#include "vector3/vector3d.h"
#include "vector3/vector3i.h"

vector3d rotation_to_direction(vector3d rotation);
float clamp(float value, float min, float max);
void vector3f_print(vector3f v);
void vector3d_print(vector3d v);
void vector3i_print(vector3i v);
int sign(double value);
int mod(int a, int b);
double random_range(double min, double max);
double min(double a, double b);
double max(double a, double b);

#endif
