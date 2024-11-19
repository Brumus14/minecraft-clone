#include "tilemap.h"

#include <stdio.h>

void tilemap_init(tilemap *tilemap, char *texture_path,
                  texture_filter texture_filter, int width, int height,
                  int tile_width, int tile_height, int margin, int spacing) {
    texture_init(&tilemap->texture, texture_filter, TEXTURE_WRAP_REPEAT);
    texture_load(&tilemap->texture, texture_path);

    // remove width and height figure out using texture width and height
    tilemap->width = width;
    tilemap->height = height;
    tilemap->tile_width = tile_width;
    tilemap->tile_height = tile_height;
    tilemap->margin = margin;
    tilemap->spacing = spacing;
}

rectangle tilemap_get_tile_rectangle(tilemap *tilemap, int tile_index) {
    rectangle rectangle;

    int tilemap_width = tilemap->margin * 2 +
                        tilemap->tile_width * tilemap->width +
                        tilemap->spacing * (tilemap->width - 1);
    int tilemap_height = tilemap->margin * 2 +
                         tilemap->tile_height * tilemap->height +
                         tilemap->spacing * (tilemap->height - 1);

    int tile_x = tile_index % tilemap->width;
    int tile_y = tile_index / tilemap->width;

    rectangle.x = (tilemap->margin + tile_x * tilemap->tile_width +
                   tile_x * tilemap->spacing) *
                  (1.0 / tilemap_width);
    rectangle.y = (tilemap->margin + tile_y * tilemap->tile_height +
                   tile_y * tilemap->spacing) *
                  (1.0 / tilemap_height);
    rectangle.width = tilemap->tile_width * (1.0 / tilemap_width);
    rectangle.height = tilemap->tile_height * (1.0 / tilemap_height);

    return rectangle;
}

void tilemap_bind(tilemap *tilemap) {
    texture_bind(&tilemap->texture);
}
