#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cglm/cglm.h"
#include "cglm/io.h"
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    set_viewport(0, 0, width, height);
}

int main() {
    window window;
    init_window(&window, 400, 400, "minecraft!");

    init_renderer();

    set_viewport(0, 0, 800, 600);

    // Make this better
    glfwSetFramebufferSizeCallback(window.glfw_window,
                                   framebuffer_size_callback);

    set_clear_colour(0.7, 0.5, 0.3, 1.0);

    mat4 model_matrix = GLM_MAT4_IDENTITY;

    camera camera;
    camera_init(&camera, (vec3){0.0, 0.0, -3.0}, (vec3){0.0, 0.0, 0.0}, 90, 0.1,
                100.0);

    while (!window_should_close(&window)) {
        clear_colour();

        float data[3][3] = {
            {0.0, 1.0, 0.0},
            {-1.0, -1.0, 0.0},
            {1.0, -1.0, 0.0},
        };

        float cube[8][3] = {
            {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0},   {1.0, 1.0, 1.0},
            {-1.0, 1.0, 1.0},  {-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0},
            {1.0, 1.0, -1.0},  {-1.0, 1.0, -1.0},
        };

        unsigned int indices[36] = {0, 1, 3, 1, 2, 3,

                                    4, 5, 7, 5, 6, 7,

                                    0, 3, 4, 3, 7, 4,

                                    1, 5, 2, 5, 6, 2,

                                    3, 2, 7, 2, 6, 7,

                                    0, 4, 1, 1, 4, 5};

        bo vbo;
        init_bo(&vbo, BUFFER_TYPE_VERTEX);
        bind_bo(&vbo);
        upload_bo(&vbo, sizeof(cube), cube, STATIC_DRAW);

        bo ibo;
        init_bo(&ibo, BUFFER_TYPE_INDEX);
        bind_bo(&ibo);
        upload_bo(&ibo, sizeof(indices), indices, STATIC_DRAW);

        vao vao;
        init_vao(&vao);
        bind_vao(&vao);
        attrib_vao(&vao, 0, 3, FLOAT, false, 0, NULL);

        char *vertex_source = read_file("src/vertex.vert");
        char *fragment_source = read_file("src/fragment.frag");

        shader vertex_shader;
        init_shader(&vertex_shader, SHADER_TYPE_VERTEX);
        source_shader(&vertex_shader, vertex_source);
        compile_shader(&vertex_shader);

        shader fragment_shader;
        init_shader(&fragment_shader, SHADER_TYPE_FRAGMENT);
        source_shader(&fragment_shader, fragment_source);
        compile_shader(&fragment_shader);

        shader_program shader_program;
        init_shader_program(&shader_program, &vertex_shader, &fragment_shader);
        bind_attribute_shader_program(&shader_program, 0, "position");
        link_shader_program(&shader_program);
        use_shader_program(&shader_program);

        GLint model_loc =
            glGetUniformLocation(shader_program.shader_program_id, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float *)model_matrix);

        camera_update_matrices(&camera);

        delete_shader(&vertex_shader);
        delete_shader(&fragment_shader);

        bind_bo(&ibo);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

        window_swap_buffers(&window);
        glfwPollEvents();

        camera_translate(&camera, (vec3){0.01, 0.01, 0.01});
    }

    window_destroy(&window);

    glfwTerminate();

    return 0;
}
