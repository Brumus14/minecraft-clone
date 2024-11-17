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

// REMMEMBER TO AUTO BIND IN FUNCTIONS THAT ITS REQUIRED

int main() {
    window window;
    camera camera;

    window_init(&window, 400, 400, "minecraft!", &camera);
    glfwSwapInterval(0);

    camera_init(&camera, VECTOR3D_ZERO, VECTOR3D_ZERO, 60.0,
                window_get_aspect_ratio(&window), 0.1, 1000.0);

    glfwSetInputMode(window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window.glfw_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    renderer_init();
    renderer_set_clear_colour(0.53, 0.81, 0.92, 1.0);
    mat4 model_matrix = GLM_MAT4_IDENTITY;

    tilemap tilemap;
    tilemap_init(&tilemap, "res/textures/atlas.png", TEXTURE_FILTER_NEAREST, 16,
                 16);

    // make arguments const

    world world;
    world_init(&world, &tilemap); // USE VECTOR3 INT

    /*greedy_mesh_vertices_indices();*/

    player player;
    player_init(&player, (vector3d){10.0, 16.0, 10.0}, VECTOR3D_ZERO, 10, 0.05,
                &camera);

    shader_program shader_program;
    shader_program_from_files(&shader_program, "res/shaders/vertex.vert",
                              "res/shaders/fragment.frag");
    shader_program_bind_attribute(&shader_program, 0, "position");
    shader_program_link(&shader_program);
    shader_program_use(&shader_program);

    while (!window_should_close(&window)) {
        window_update_delta_time(&window);
        window_update_input(&window);
        glfwPollEvents();

        player_manage_chunks(&player, &world); // create chunk manager? ecs?
        printf("%d\n", world.chunk_count);

        printf("%f\n", 1.0 / window_get_delta_time(&window));
        renderer_clear_buffers();

        if (keyboard_key_just_down(&window.keyboard, KEYCODE_ESCAPE)) {
            window_reset_cursor(&window);
        }

        if (glfwGetMouseButton(window.glfw_window, GLFW_MOUSE_BUTTON_LEFT) ==
            GLFW_PRESS) {
            window_capture_cursor(&window);
        }

        player_handle_input(&player, &window);

        camera_set_rotation(&camera, player.rotation);
        camera_set_position(&camera, player.position);

        GLint model_loc = glGetUniformLocation(shader_program.gl_id, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float *)model_matrix);

        camera_update_matrix_uniforms(&camera);

        world_draw(&world);

        window_swap_buffers(&window);
    }

    window_destroy(&window);

    glfwTerminate();

    return 0;
}
