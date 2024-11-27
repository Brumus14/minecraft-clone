#ifndef HOTBAR_H
#define HOTBAR_H

#include "world/block.h"
#include "gui/gui.h"
#include "gui/image.h"
#include "tilemap.h"

typedef struct hotbar {
    block_type items[9];
    int current_slot;
    tilemap item_tilemap;
    gui *gui;
    gui_image hotbar_image;
    gui_image item_images[9];
    gui_image hotbar_selected_image;
} hotbar;

void hotbar_init(hotbar *hotbar, gui *gui);
block_type hotbar_get_current_item(hotbar *hotbar);
void hotbar_set_item(hotbar *hotbar, block_type item, int slot);
void hotbar_update_gui(hotbar *hotbar);

#endif
