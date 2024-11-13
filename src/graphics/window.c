#include "window.h"

bool glfw_initialised = false;

keycode glfw_keycode_to_keycode(int key) {
    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
        return key - 48;
    } else if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
        return (key - 65) + KEYCODE_A;
    } else if (key == GLFW_KEY_SPACE) {
        return KEYCODE_SPACE;
    } else if (key == GLFW_KEY_ESCAPE) {
        return KEYCODE_ESCAPE;
    } else if (key == GLFW_KEY_LEFT_SHIFT) {
        return KEYCODE_LEFT_SHIFT;
    } else if (key >= GLFW_KEY_RIGHT && key <= GLFW_KEY_UP) {
        return (key - GLFW_KEY_RIGHT) + KEYCODE_ARROW_RIGHT;
    }

    return -1;
}

void glfw_framebuffer_size_callback(GLFWwindow *glfw_window, int width,
                                    int height) {
    window *window_pointer = (window *)glfwGetWindowUserPointer(glfw_window);

    renderer_set_viewport(0, 0, width, height);
    glfwGetWindowSize(window_pointer->glfw_window, &window_pointer->width,
                      &window_pointer->height);
    camera_set_aspect_ratio(window_pointer->camera,
                            (float)window_pointer->width /
                                window_pointer->height);
}

void glfw_cursor_pos_callback(GLFWwindow *glfw_window, double xpos,
                              double ypos) {
    window *window_pointer = (window *)glfwGetWindowUserPointer(glfw_window);

    mouse_set_position(&window_pointer->mouse, (vector2){xpos, ypos});
}

void glfw_key_callback(GLFWwindow *glfw_window, int key, int scancode,
                       int action, int mods) {
    window *window_pointer = (window *)glfwGetWindowUserPointer(glfw_window);

    key_state state;

    if (action == GLFW_PRESS) {
        state = KEY_STATE_DOWN;
    } else if (action == GLFW_RELEASE) {
        state = KEY_STATE_UP;
    } else {
        return;
    }

    keycode keycode = glfw_keycode_to_keycode(key);

    if (keycode < KEYCODE_LAST) {
        keyboard_set_key(&window_pointer->keyboard,
                         glfw_keycode_to_keycode(key), state);
    }
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

    keyboard_init(&window->keyboard);
    mouse_init(&window->mouse);

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
    glfwSetKeyCallback(window->glfw_window, glfw_key_callback);
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

void window_reset_cursor(window *window) {
    glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void window_capture_cursor(window *window) {
    glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

float window_get_delta_time(window *window) {
    return window->delta_time;
}

void window_update_delta_time(window *window) {
    window->delta_time = glfwGetTime();

    glfwSetTime(0);
}

void window_set_framebuffer_size_callback(
    window *window, window_framebuffer_size_callback function) {
    window->framebuffer_size_callback = function;
}

void window_set_cursor_pos_callback(window *window,
                                    window_cursor_pos_callback function) {
    window->cursor_pos_callback = function;
}
