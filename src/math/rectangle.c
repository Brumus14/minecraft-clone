#include "rectangle.h"

void rectangle_init(rectangle *rectangle, float x, float y, float width,
                    float height) {
    rectangle->x = x;
    rectangle->y = y;
    rectangle->width = width;
    rectangle->height = height;
}
