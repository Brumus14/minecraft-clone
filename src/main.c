#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdio.h>
#include "util.h"
#include "graphics/graphics.h"
#include "world/block.h"
#include "world/chunk.h"
#include "tilemap.h"
#include "player.h"
#include "world/world.h"
#include "math/math_util.h"
#include "gui/gui.h"
#include "gui/gui_image.h"

// REMMEMBER TO AUTO BIND IN FUNCTIONS THAT ITS REQUIRED
// make arguments const
// make camera prepare draw function
// better gui

int main() {
    window window;
    camera camera;
    gui gui;

    window_init(&window, 400, 400, "minecraft!", &camera);
    glfwSwapInterval(0);

    renderer_init();
    renderer_set_clear_colour(0.53, 0.81, 0.92, 1.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    camera_init(&camera, VECTOR3D_ZERO, VECTOR3D_ZERO, 90.0,
                window_get_aspect_ratio(&window), 0.1, 1000.0);

    gui_init(&gui, &window);

    gui_image crosshair;
    gui_image_init(&crosshair, "res/textures/crosshair.png", (vector2d){0, 0},
                   (vector2d){0.04, 0.04});

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
        renderer_clear_buffers();

        window_update_delta_time(&window);
        window_update_input(&window);

        player_manage_chunks(&player, &world); // create chunk manager? ecs?

        /*printf("%f\n", 1.0 / window_get_delta_time(&window));*/

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

        player_handle_input(&player, &window);

        camera_set_rotation(&camera, player.rotation);
        camera_set_position(&camera, player.position);

        camera_prepare_draw(&camera);

        world_draw(&world);

        gui_prepare_draw(&gui);

        gui_image_draw(&crosshair);

        window_swap_buffers(&window);
    }

    window_destroy(&window);

    glfwTerminate();

    return 0;
}
