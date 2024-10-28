#include "window.h"

bool glfw_initialised = false;

void glfw_framebuffer_size_callback(GLFWwindow *glfw_window, int width,
                                    int height) {
    window *window_pointer = (window *)glfwGetWindowUserPointer(glfw_window);
    window_pointer->framebuffer_size_callback(window_pointer, width, height);
}

void glfw_cursor_pos_callback(GLFWwindow *glfw_window, double xpos,
                              double ypos) {
    window *window_pointer = (window *)glfwGetWindowUserPointer(glfw_window);
    window_pointer->cursor_pos_callback(window_pointer, xpos, ypos);
}

void glfw_init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void window_init(window *window, int width, int height, char *title,
                 camera *camera) {
    window->width = width;
    window->height = height;
    window->title = title;
    window->camera = camera;

    if (!glfw_initialised) {
        glfw_init();
    }

    window->glfw_window = glfwCreateWindow(window->width, window->height,
                                           window->title, NULL, NULL);
    glfwMakeContextCurrent(window->glfw_window);

    glfwSetWindowUserPointer(window->glfw_window, window);

    glfwSetFramebufferSizeCallback(window->glfw_window,
                                   glfw_framebuffer_size_callback);
    glfwSetCursorPosCallback(window->glfw_window, glfw_cursor_pos_callback);
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

void window_set_framebuffer_size_callback(
    window *window, window_framebuffer_size_callback function) {
    window->framebuffer_size_callback = function;
}

void window_set_cursor_pos_callback(window *window,
                                    window_cursor_pos_callback function) {
    window->cursor_pos_callback = function;
}
