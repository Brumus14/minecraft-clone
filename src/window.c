#include "window.h"

bool glfw_initialised = false;

void init_glfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void init_window(window *window, int width, int height, char *title) {
    window->width = width;
    window->height = height;
    window->title = title;

    if (!glfw_initialised) {
        init_glfw();
    }

    window->glfw_window = glfwCreateWindow(window->width, window->height,
                                           window->title, NULL, NULL);
    glfwMakeContextCurrent(window->glfw_window);
}

bool window_should_close(window *window) {
    return glfwWindowShouldClose(window->glfw_window);
}

void window_swap_buffers(window *window) {
    glfwSwapBuffers(window->glfw_window);
}

void window_destroy(window *window) {
    glfwDestroyWindow(window->glfw_window);
}

float window_get_aspect_ratio(window *window) {
    return (float)window->width / window->height;
}

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
}

void
