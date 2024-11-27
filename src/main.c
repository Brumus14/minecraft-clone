#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdio.h>
#include <unistd.h>
#include "util.h"
#include "graphics/graphics.h"
#include "world/block.h"
#include "world/chunk.h"
#include "tilemap.h"
#include "player.h"
#include "world/world.h"
#include "math/math_util.h"
#include "gui/gui.h"
#include "gui/image.h"
#include "stopwatch.h"
#include "hotbar.h"

// REMMEMBER TO AUTO BIND IN FUNCTIONS THAT ITS REQUIRED
// make arguments const
// make camera prepare draw function
// better gui

int main() {
    stopwatch timer;

    window window;
    camera camera;
    gui gui;

    window_init(&window, 400, 400, "minecraft!", &camera);
    glfwSwapInterval(0);

    renderer_init();
    renderer_set_clear_colour(0.53, 0.81, 0.92, 1.0);

    camera_init(&camera, VECTOR3D_ZERO, VECTOR3D_ZERO, 90.0,
                window_get_aspect_ratio(&window), 0.1, 1000.0);

    gui_init(&gui, &window);

    gui_image crosshair;
    gui_image_init(&crosshair, "res/textures/crosshair.png",
                   (rectangle){0, 0, 1, 1}, VECTOR2D_ZERO, (vector2d){16, 16},
                   GUI_ELEMENT_ORIGIN_CENTER_CENTER, GUI_ELEMENT_LAYER_0);

    gui_add_image(&gui, &crosshair);

    hotbar hotbar;
    hotbar_init(&hotbar, &gui);

    tilemap tilemap;
    tilemap_init(&tilemap, "res/textures/atlas.png", TEXTURE_FILTER_NEAREST, 16,
                 16, 16, 16, 1, 2);

    world world;
    world_init(&world, &tilemap);

    /*greedy_mesh_vertices_indices();*/

    // 21474836.0
    // 2147483.0
    player player;
    player_init(&player, (vector3d){8.0, 4.0, 8.0}, VECTOR3D_ZERO, 5, 0.05,
                &camera);

    while (!window_should_close(&window)) {
        glfwSwapInterval(0);
        stopwatch_start(&timer);
        renderer_clear_buffers();
        stopwatch_end(&timer);
        printf("TIMER 1: %f\n", stopwatch_time(&timer));

        stopwatch_start(&timer);
        window_update_delta_time(&window);
        window_update_input(&window);
        stopwatch_end(&timer);
        printf("TIMER 2: %f\n", stopwatch_time(&timer));

        if (window.mouse.scroll_offset.y > 0) {
            hotbar.current_slot++;
            hotbar.current_slot = mod(hotbar.current_slot, 9);
        } else if (window.mouse.scroll_offset.y < 0) {
            hotbar.current_slot--;
            hotbar.current_slot = mod(hotbar.current_slot, 9);
        }

        gui_image_set_position(
            &crosshair, (vector2d){(double)window.width / 2,
                                   (double)window.height /
                                       2}); // only run when window size changed
        hotbar_update_gui(&hotbar);

        stopwatch_start(&timer);
        player_manage_chunks(&player,
                             &world); // create chunk manager? ecs? SLOW
        stopwatch_end(&timer);
        printf("TIMER 3: %f\n", stopwatch_time(&timer));

        /*printf("%f\n", 1.0 / window_get_delta_time(&window));*/

        stopwatch_start(&timer);
        if (keyboard_key_just_down(&window.keyboard, KEYCODE_ESCAPE)) {
            window_reset_cursor(&window);
        }

        if (mouse_button_just_down(&window.mouse, MOUSE_BUTTON_LEFT)) {
            window_capture_cursor(&window);
            player_destroy_block(&player, &world);
        }

        if (mouse_button_just_down(&window.mouse, MOUSE_BUTTON_RIGHT)) {
            player_place_block(&player, &world, BLOCK_TYPE_LOG);
        }
        stopwatch_end(&timer);
        printf("TIMER 4: %f\n", stopwatch_time(&timer));

        stopwatch_start(&timer);
        player_handle_input(&player, &window);
        stopwatch_end(&timer);
        printf("TIMER 5: %f\n", stopwatch_time(&timer));

        stopwatch_start(&timer);
        camera_set_rotation(&camera, player.rotation);
        camera_set_position(&camera, player.position);
        stopwatch_end(&timer);
        printf("TIMER 6: %f\n", stopwatch_time(&timer));

        stopwatch_start(&timer);
        camera_prepare_draw(&camera);
        stopwatch_end(&timer);
        printf("TIMER 7: %f\n", stopwatch_time(&timer));

        stopwatch_start(&timer);
        world_draw(&world); // SLOW
        stopwatch_end(&timer);
        printf("TIMER 8: %f\n", stopwatch_time(&timer));

        renderer_clear_depth_buffer();
        stopwatch_start(&timer);
        gui_draw(&gui);
        stopwatch_end(&timer);
        printf("TIMER 9: %f\n", stopwatch_time(&timer));

        stopwatch_start(&timer);
        window_swap_buffers(&window);
        stopwatch_end(&timer);
        printf("TIMER 10: %f\n", stopwatch_time(&timer));
    }

    window_destroy(&window);

    glfwTerminate(); // move this somewhere

    return 0;
}
