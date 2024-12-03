#ifndef TILEMAP_H
#define TILEMAP_H

#include "graphics/texture.h"
#include "math/rectangle.h"

typedef struct tilemap {
    texture texture;
    int width;
    int height;
    int tile_width;
    int tile_height;
    int margin;
    int spacing;
} tilemap;

void tilemap_init(tilemap *tilemap, char *texture_path,
                  texture_filter texture_filter, int tile_width,
                  int tile_height, int margin, int spacing);
rectangle tilemap_get_tile_rectangle(tilemap *tilemap, int tile_index);
void tilemap_bind(tilemap *tilemap);

#endif
