#include "tilemap.h"

#include <stdio.h>

void tilemap_init(tilemap *tilemap, char *texture_path,
                  texture_filter texture_filter, int width, int height) {
    texture_init(&tilemap->texture, texture_filter);
    texture_load(&tilemap->texture, texture_path);

    tilemap->width = width;
    tilemap->height = height;
}

rectangle tilemap_get_tile_rectangle(tilemap *tilemap, int tile_index) {
    rectangle rectangle;

    float tile_width = 1.0 / tilemap->width;
    float tile_height = 1.0 / tilemap->height;

    int tile_x = tile_index % tilemap->width;
    int tile_y = tile_index / tilemap->width;

    rectangle_init(&rectangle, tile_x * tile_width, tile_y * tile_height,
                   tile_width, tile_height);

    return rectangle;
}

void tilemap_bind(tilemap *tilemap) {
    texture_bind(&tilemap->texture);
}
