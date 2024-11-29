#ifndef CUBOID_H
#define CUBOID_H

typedef struct cuboid {
    float x;
    float y;
    float z;
    float width;
    float height;
    float depth;
} cuboid;

void cuboid_init(cuboid *cuboid, float x, float y, float z, float width,
                 float height, float depth);

#endif
