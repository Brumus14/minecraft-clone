#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "graphics/graphics.h"
#include "world/block.h"
#include "world/chunk.h"
#include "tilemap.h"

// REMMEMBER TO AUTO BIND IN FUNCTIONS THAT ITS REQUIRED

const double SENSITIVITY = 0.1;

void framebuffer_size_callback(window *window, int width, int height) {
    renderer_set_viewport(0, 0, width, height);
    glfwGetWindowSize(window->glfw_window, &window->width, &window->height);
    camera_set_aspect_ratio(window->camera,
                            (float)window->width / window->height);
}

void cursor_pos_callback(window *window, double x_pos, double y_pos) {
    static double previous_x_pos;
    static double previous_y_pos;

    camera_rotate(window->camera,
                  (vector3){(previous_y_pos - y_pos) * SENSITIVITY,
                            (x_pos - previous_x_pos) * SENSITIVITY, 0.0});
    previous_x_pos = x_pos;
    previous_y_pos = y_pos;
}

int main() {
    window window;
    camera camera;

    window_init(&window, 400, 400, "minecraft!", &camera);

    camera_init(&camera, (vector3){0.0, 0.0, 3.0}, VECTOR3_ZERO, 60.0,
                window_get_aspect_ratio(&window), 0.1, 100.0);

    window_set_framebuffer_size_callback(&window, framebuffer_size_callback);
    window_set_cursor_pos_callback(&window, cursor_pos_callback);

    glfwSetInputMode(window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    renderer_init();
    renderer_set_clear_colour(0.53, 0.81, 0.92, 1.0);
    mat4 model_matrix = GLM_MAT4_IDENTITY;

    tilemap tilemap;
    tilemap_init(&tilemap, "res/textures/atlas.png", TEXTURE_FILTER_NEAREST, 16,
                 16);

    // make arguments const

    block blocks[5];

    /*for (int y = 0; y < 10; y++) {*/
    /*    for (int x = 0; x < 10; x++) {*/
    /*        block_init(&blocks[y * 10 + x], (vector3){x, 0.0, y},*/
    /*                   BLOCK_TYPE_GRASS,*/
    /*                   (bool[]){true, true, true, true, true, true},
     * &tilemap);*/
    /*    }*/
    /*}*/
    /**/
    /*for (int y = 0; y < 10; y++) {*/
    /*    for (int x = 0; x < 10; x++) {*/
    /*        block_init(&blocks[100 + y * 10 + x], (vector3){x, -1.0, y},*/
    /*                   BLOCK_TYPE_DIRT, ACTIVE_FACES_ALL, &tilemap);*/
    /*    }*/
    /*}*/
    /**/
    /*block_init(&blocks[21], (vector3){1.0, 0.0, 2.0}, BLOCK_TYPE_EMPTY,*/
    /*           ACTIVE_FACES_ALL, &tilemap);*/

    /*chunk chunk;*/
    /*chunk_init(&chunk, &tilemap);*/
    /*chunk_calculate_active_faces(&chunk);*/

    /*for (int y = 0; y < 3; y++) {*/
    for (int x = 0; x < 5; x++) {
        if (x == 2) {

            block_init(&blocks[x], (vector3){x, 0.0, 0.0}, BLOCK_TYPE_GRASS,
                       (bool[]){true, false, true, true, true, true}, &tilemap);
        } else {
            block_init(&blocks[x], (vector3){x, 0.0, 0.0}, BLOCK_TYPE_GRASS,
                       (bool[]){true, true, true, true, true, true}, &tilemap);
        }
    }
    /*}*/

    /*block_init(&blocks[0], (vector3){0.0, 0.0, 0.0}, BLOCK_TYPE_GRASS,*/
    /*           (bool[]){true, false, true, true, true, true}, &tilemap);*/

    shader_program shader_program;
    shader_program_from_files(&shader_program, "res/shaders/vertex.vert",
                              "res/shaders/fragment.frag");
    shader_program_bind_attribute(&shader_program, 0, "position");
    shader_program_link(&shader_program);
    shader_program_use(&shader_program);

    while (!window_should_close(&window)) {
        renderer_clear_buffers();

        float camera_speed = 0.05;
        vector3 camera_delta;
        vector3_init(&camera_delta, 0.0, 0.0, 0.0);

        vector3 rotation_delta;
        vector3_init(&rotation_delta, 0.0, 0.0, 0.0);

        if (glfwGetKey(window.glfw_window, GLFW_KEY_S)) {
            camera_delta.z += 1.0;
        }

        if (glfwGetKey(window.glfw_window, GLFW_KEY_W)) {
            camera_delta.z += -1.0;
        }

        if (glfwGetKey(window.glfw_window, GLFW_KEY_A)) {
            camera_delta.x += -1.0;
        }

        if (glfwGetKey(window.glfw_window, GLFW_KEY_D)) {
            camera_delta.x += 1.0;
        }

        if (glfwGetKey(window.glfw_window, GLFW_KEY_SPACE)) {
            camera_delta.y += 1.0;
        }

        if (glfwGetKey(window.glfw_window, GLFW_KEY_LEFT_SHIFT)) {
            camera_delta.y += -1.0;
        }

        if (glfwGetKey(window.glfw_window, GLFW_KEY_LEFT)) {
            rotation_delta.y += -1.0;
        }

        if (glfwGetKey(window.glfw_window, GLFW_KEY_RIGHT)) {
            rotation_delta.y += 1.0;
        }

        if (glfwGetKey(window.glfw_window, GLFW_KEY_UP)) {
            rotation_delta.x += 1.0;
        }

        if (glfwGetKey(window.glfw_window, GLFW_KEY_DOWN)) {
            rotation_delta.x += -1.0;
        }

        if (glfwGetKey(window.glfw_window, GLFW_KEY_ESCAPE)) {
            window_reset_cursor(&window);
        }

        if (glfwGetMouseButton(window.glfw_window, GLFW_MOUSE_BUTTON_LEFT) ==
            GLFW_PRESS) {
            window_capture_cursor(&window);
        }

        camera_rotate(&camera, rotation_delta);

        vector3_normalise(&camera_delta);

        camera_delta.x *= camera_speed;
        camera_delta.y *= camera_speed;
        camera_delta.z *= camera_speed;

        camera_translate(&camera, camera_delta);

        GLint model_loc = glGetUniformLocation(shader_program.gl_id, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float *)model_matrix);

        camera_update_matrix_uniforms(&camera);

        for (int i = 0; i < 5; i++) {
            block_draw(&blocks[i]);
        }

        /*chunk_draw(&chunk);*/

        window_swap_buffers(&window);
        glfwPollEvents();
    }

    window_destroy(&window);

    glfwTerminate();

    return 0;
}
