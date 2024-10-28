#ifndef RECTANGLE_H
#define RECTANGLE_H

typedef struct rectangle {
    float x;
    float y;
    float width;
    float height;
} rectangle;

void rectangle_init(rectangle *rectangle, float x, float y, float width,
                    float height);

#endif
