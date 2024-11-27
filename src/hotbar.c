#include "hotbar.h"

void hotbar_init(hotbar *hotbar, gui *gui) {
    hotbar->current_slot = 0;

    tilemap_init(&hotbar->item_tilemap, "res/textures/raw_icons.png",
                 TEXTURE_FILTER_NEAREST, 4, 4, 32, 32, 0, 0);

    hotbar->gui = gui;

    // set texture rectangle by default then use function to set if needed
    gui_image_init(&hotbar->hotbar_image, "res/textures/hotbar.png",
                   (rectangle){0, 0, 1, 1},
                   (vector2d){(double)hotbar->gui->window->width / 2,
                              hotbar->gui->window->height - 2},
                   vector2d_scalar_multiply((vector2d){308, 36}, 2),
                   GUI_ELEMENT_ORIGIN_CENTER_BOTTOM, GUI_ELEMENT_LAYER_0);
    gui_add_image(gui, &hotbar->hotbar_image);

    for (int i = 0; i < 9; i++) {
        gui_image_init(&hotbar->item_images[i], "res/textures/raw_icons.png",
                       tilemap_get_tile_rectangle(&hotbar->item_tilemap, i % 2),
                       (vector2d){(double)gui->window->width / 2,
                                  gui->window->height - 2 * 2},
                       vector2d_scalar_multiply((vector2d){32, 32}, 2),
                       GUI_ELEMENT_ORIGIN_CENTER_BOTTOM, GUI_ELEMENT_LAYER_1);
        gui_add_image(gui, &hotbar->item_images[i]);
    }

    gui_image_init(
        &hotbar->hotbar_selected_image, "res/textures/hotbar_selected.png",
        (rectangle){0, 0, 1, 1},
        (vector2d){(double)gui->window->width / 2, gui->window->height - 2 * 2},
        vector2d_scalar_multiply((vector2d){36, 36}, 2),
        GUI_ELEMENT_ORIGIN_CENTER_BOTTOM, GUI_ELEMENT_LAYER_2);
    gui_add_image(gui, &hotbar->hotbar_selected_image);
}

void hotbar_update_gui(hotbar *hotbar) {
    gui_image_set_position(
        &hotbar->hotbar_image,
        (vector2d){round((double)hotbar->gui->window->width / 2),
                   hotbar->gui->window->height - 2 * 2});

    for (int i = 0; i < 9; i++) {
        double x =
            round((double)hotbar->gui->window->width / 2) + ((i - 4) * 34 * 2);

        gui_image_set_position(
            &hotbar->item_images[i],
            (vector2d){x, hotbar->gui->window->height - 4 * 2});
    }

    double selected_x = round((double)hotbar->gui->window->width / 2 +
                              (hotbar->current_slot - 4) * 34 * 2);

    gui_image_set_position(
        &hotbar->hotbar_selected_image,
        (vector2d){selected_x, hotbar->gui->window->height - 2 * 2});
}

block_type hotbar_get_current_item(hotbar *hotbar) {
    return hotbar->items[hotbar->current_slot];
}

void hotbar_set_item(hotbar *hotbar, block_type item, int slot) {
    hotbar->items[slot] = item;
}
