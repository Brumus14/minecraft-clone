#ifndef TILEMAP_H
#define TILEMAP_H

#include "graphics/texture.h"
#include "math/rectangle.h"

typedef struct tilemap {
    texture texture;
    texture_filter texture_filter;
    int width;
    int height;
} tilemap;

void tilemap_init(tilemap *tilemap, char *texture_path,
                  texture_filter texture_filter, int width, int height);
rectangle tilemap_get_tile_rectangle(tilemap *tilemap, int tile_index);
void tilemap_bind(tilemap *tilemap);

#endif
