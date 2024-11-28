#include "hotbar.h"

// separate window resizing stuff
void hotbar_update_gui(hotbar *hotbar) {
    gui_image_set_position(
        &hotbar->hotbar_image,
        (vector2d){round((double)hotbar->gui->window->width / 2),
                   hotbar->gui->window->height - 2 * 2});

    for (int i = 0; i < 9; i++) {
        if (hotbar->items[i] == ITEM_TYPE_EMPTY) {
            hotbar->item_images[i].visible = false;

            continue;
        }

        hotbar->item_images[i].visible = true;

        double x =
            round((double)hotbar->gui->window->width / 2) + ((i - 4) * 34 * 2);

        gui_image_set_position(
            &hotbar->item_images[i],
            (vector2d){x, hotbar->gui->window->height - 4 * 2});

        gui_image_set_texture_rectangle(
            &hotbar->item_images[i],
            tilemap_get_tile_rectangle(
                &hotbar->item_tilemap,
                item_type_to_texture_index(hotbar->items[i])));
    }

    double selected_x = round((double)hotbar->gui->window->width / 2 +
                              (hotbar->current_slot - 4) * 34 * 2);

    gui_image_set_position(
        &hotbar->hotbar_selected_image,
        (vector2d){selected_x, hotbar->gui->window->height - 2 * 2});
}

void hotbar_init(hotbar *hotbar, gui *gui) {
    hotbar->current_slot = 0;

    tilemap_init(&hotbar->item_tilemap, "res/textures/raw_icons.png",
                 TEXTURE_FILTER_NEAREST, 4, 4, 32, 32, 1, 2);

    for (int i = 0; i < 9; i++) {
        hotbar->items[i] = ITEM_TYPE_EMPTY;
    }

    hotbar->gui = gui;

    // multiply scale by the size
    gui_image_init(&hotbar->hotbar_image, "res/textures/hotbar.png",
                   VECTOR2D_ZERO, (vector2d){308 * 2, 36 * 2},
                   GUI_ELEMENT_ORIGIN_CENTER_BOTTOM, GUI_ELEMENT_LAYER_0);

    for (int i = 0; i < 9; i++) {
        gui_image_init(&hotbar->item_images[i], "res/textures/raw_icons.png",
                       VECTOR2D_ZERO, (vector2d){32 * 2, 32 * 2},
                       GUI_ELEMENT_ORIGIN_CENTER_BOTTOM, GUI_ELEMENT_LAYER_2);
    }

    gui_image_init(&hotbar->hotbar_selected_image,
                   "res/textures/hotbar_selected.png", VECTOR2D_ZERO,
                   (vector2d){36 * 2, 36 * 2}, GUI_ELEMENT_ORIGIN_CENTER_BOTTOM,
                   GUI_ELEMENT_LAYER_1);

    hotbar_update_gui(hotbar);

    gui_add_image(gui, &hotbar->hotbar_image);

    for (int i = 0; i < 9; i++) {
        gui_add_image(gui, &hotbar->item_images[i]);
    }

    gui_add_image(gui, &hotbar->hotbar_selected_image);
}

item_type hotbar_get_current_item(hotbar *hotbar) {
    return hotbar->items[hotbar->current_slot];
}

void hotbar_set_item(hotbar *hotbar, int slot, item_type item) {
    hotbar->items[slot] = item;
    hotbar_update_gui(hotbar);
}
