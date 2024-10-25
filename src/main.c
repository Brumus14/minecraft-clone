#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "bo.h"
#include "vao.h"
#include "shader.h"
#include "shader_program.h"
#include "renderer.h"
#include "window.h"
#include "camera.h"

double pxpos;
double pypos;

void framebuffer_size_callback(window *window, int width, int height) {
    renderer_set_viewport(0, 0, width, height);
    glfwGetWindowSize(window->glfw_window, &window->width, &window->height);
    camera_set_aspect_ratio(window->camera,
                            (float)window->width / window->height);
}

void cursor_pos_callback(window *window, double xpos, double ypos) {
    camera_rotate(window->camera,
                  (vector3){(pypos - ypos) * 0.1, (xpos - pxpos) * 0.1, 0.0});
    pxpos = xpos;
    pypos = ypos;
}

int main() {
    window window;
    camera camera;

    window_init(&window, 400, 400, "minecraft!", &camera);

    camera_init(&camera, (vector3){0.0, 0.0, 3.0}, (vector3){0.0, -90.0, 0.0},
                60.0, window_get_aspect_ratio(&window), 0.1, 100.0);

    window_set_framebuffer_size_callback(&window, framebuffer_size_callback);
    window_set_cursor_pos_callback(&window, cursor_pos_callback);

    glfwSetInputMode(window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    renderer_init();

    renderer_set_clear_colour(0.7, 0.5, 0.3, 1.0);

    mat4 model_matrix = GLM_MAT4_IDENTITY;

    int width, height, nrChannels;
    unsigned char *texture_data =
        stbi_load("res/grass.jpg", &width, &height, &nrChannels, 0);

    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, texture_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    stbi_image_free(texture_data);

    while (!window_should_close(&window)) {
        renderer_clear_colour();

        float camera_speed = 0.03;
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

        camera_rotate(&camera, rotation_delta);

        vector3_normalise(&camera_delta);

        camera_delta.x *= camera_speed;
        camera_delta.y *= camera_speed;
        camera_delta.z *= camera_speed;

        camera_translate(&camera, camera_delta);

        float cube[8][5] = {
            {-1.0, -1.0, 1.0, 0.0, 0.0},  {1.0, -1.0, 1.0, 1.0, 0.0},
            {1.0, 1.0, 1.0, 1.0, 1.0},    {-1.0, 1.0, 1.0, 0.0, 1.0},
            {-1.0, -1.0, -1.0, 0.0, 0.0}, {1.0, -1.0, -1.0, 0.0, 0.0},
            {1.0, 1.0, -1.0, 0.0, 0.0},   {-1.0, 1.0, -1.0, 0.0, 0.0},
        };

        unsigned int indices[36] = {0, 1, 3, 1, 2, 3,

                                    4, 5, 7, 5, 6, 7,

                                    0, 3, 4, 3, 7, 4,

                                    1, 5, 2, 5, 6, 2,

                                    3, 2, 7, 2, 6, 7,

                                    0, 4, 1, 1, 4, 5};

        bo vbo;
        bo_init(&vbo, BO_TYPE_VERTEX);
        bo_bind(&vbo);
        bo_upload(&vbo, sizeof(cube), cube, BO_USAGE_STATIC_DRAW);

        bo ibo;
        bo_init(&ibo, BO_TYPE_INDEX);
        bo_bind(&ibo);
        bo_upload(&ibo, sizeof(indices), indices, BO_USAGE_STATIC_DRAW);

        vao vao;
        vao_init(&vao);
        vao_bind(&vao);
        vao_attrib(&vao, 0, 3, VAO_TYPE_FLOAT, false, 5 * sizeof(float), 0);
        vao_attrib(&vao, 1, 2, VAO_TYPE_FLOAT, false, 5 * sizeof(float),
                   (void *)(3 * sizeof(float)));

        shader_program shader_program;
        shader_program_from_files(&shader_program, "src/vertex.vert",
                                  "src/fragment.frag");
        shader_program_bind_attribute(&shader_program, 0, "position");
        shader_program_link(&shader_program);
        shader_program_use(&shader_program);

        GLint model_loc = glGetUniformLocation(shader_program.gl_id, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float *)model_matrix);

        camera_update_matrix_uniforms(&camera);

        /*glActiveTexture(GL_TEXTURE0);*/
        /*glBindTexture(GL_TEXTURE_2D, texture);*/
        /**/
        /*GLint texture_loc = glGetUniformLocation(*/
        /*    shader_program.shader_program_id, "ourTexture");*/
        /*glUniform1i(texture_loc, 0);*/

        bo_bind(&ibo);
        bo_bind(&vbo);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        window_swap_buffers(&window);
        glfwPollEvents();
    }

    window_destroy(&window);

    glfwTerminate();

    return 0;
}
