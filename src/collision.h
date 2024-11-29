#ifndef COLLISION_H
#define COLLISION_H

#include <stdbool.h>
#include "math/rectangle.h"
#include "math/cuboid.h"

bool collision_aabb_2d(rectangle r1, rectangle r2);
bool collision_aabb_3d(cuboid c1, cuboid c2);

#endif
