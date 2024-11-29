#include "window.h"

bool glfw_initialised = false;

keycode glfw_to_keycode(int key) {
    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
        return (key - GLFW_KEY_0) + KEYCODE_0;
    }

    else if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
        return (key - GLFW_KEY_A) + KEYCODE_A;
    }

    else if (key == GLFW_KEY_SPACE) {
        return KEYCODE_SPACE;
    }

    else if (key >= GLFW_KEY_ESCAPE && key <= GLFW_KEY_BACKSPACE) {
        return (key - GLFW_KEY_ESCAPE) + KEYCODE_ESCAPE;
    }

    else if (key >= GLFW_KEY_RIGHT && key <= GLFW_KEY_UP) {
        return (key - GLFW_KEY_RIGHT) + KEYCODE_ARROW_RIGHT;
    }

    else if (key >= GLFW_KEY_LEFT_SHIFT && key <= GLFW_KEY_RIGHT_SUPER) {
        return (key - GLFW_KEY_LEFT_SHIFT) + KEYCODE_LEFT_SHIFT;
    }

    return -1;
}

int keycode_to_glfw(keycode key) {
    if (key >= KEYCODE_0 && key <= KEYCODE_9) {
        return (key - KEYCODE_0) + GLFW_KEY_0;
    }

    else if (key >= KEYCODE_A && key <= KEYCODE_Z) {
        return (key - KEYCODE_A) + GLFW_KEY_A;
    }

    else if (key == KEYCODE_SPACE) {
        return GLFW_KEY_SPACE;
    }

    else if (key >= KEYCODE_ESCAPE && key <= KEYCODE_BACKSPACE) {
        return (key - KEYCODE_ESCAPE) + GLFW_KEY_ESCAPE;
    }

    else if (key >= KEYCODE_ARROW_RIGHT && key <= KEYCODE_ARROW_UP) {
        return (key - KEYCODE_ARROW_RIGHT) + GLFW_KEY_RIGHT;
    }

    else if (key >= KEYCODE_LEFT_SHIFT && key <= KEYCODE_RIGHT_SUPER) {
        return (key - KEYCODE_LEFT_SHIFT) + GLFW_KEY_LEFT_SHIFT;
    }

    return -1;
}

mouse_button glfw_to_mouse_button(int button) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        return MOUSE_BUTTON_LEFT;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        return MOUSE_BUTTON_MIDDLE;
    case GLFW_MOUSE_BUTTON_RIGHT:
        return MOUSE_BUTTON_RIGHT;
    }

    return -1;
}

int mouse_button_to_glfw(mouse_button button) {
    switch (button) {
    case MOUSE_BUTTON_LEFT:
        return GLFW_MOUSE_BUTTON_LEFT;
    case MOUSE_BUTTON_MIDDLE:
        return GLFW_MOUSE_BUTTON_MIDDLE;
    case MOUSE_BUTTON_RIGHT:
        return GLFW_MOUSE_BUTTON_RIGHT;
    default:
        break;
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

void glfw_scroll_callback(GLFWwindow *glfw_window, double xoffset,
                          double yoffset) {
    window *window_pointer = (window *)glfwGetWindowUserPointer(glfw_window);

    /*if (window_pointer->scroll_callback != NULL) {*/
    /*    window_pointer->scroll_callback(window_pointer, xoffset, yoffset);*/
    /*}*/

    window_pointer->mouse.scroll_offset.x += xoffset;
    window_pointer->mouse.scroll_offset.y += yoffset;
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
    // work on callbacks
    window->scroll_callback = NULL;

    keyboard_init(&window->keyboard);
    mouse_init(&window->mouse);

    if (!glfw_initialised) {
        glfw_init();
    }

    window->glfw_window = glfwCreateWindow(window->width, window->height,
                                           window->title, NULL, NULL);
    glfwMakeContextCurrent(window->glfw_window);

    glfwSetWindowUserPointer(window->glfw_window, window);

    glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window->glfw_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetFramebufferSizeCallback(window->glfw_window,
                                   glfw_framebuffer_size_callback);
    /*glfwSetCursorPosCallback(window->glfw_window, glfw_cursor_pos_callback);*/
    /*glfwSetKeyCallback(window->glfw_window, glfw_key_callback);*/
    glfwSetScrollCallback(window->glfw_window, glfw_scroll_callback);
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

double window_get_delta_time(window *window) {
    return window->delta_time;
}

void window_update_delta_time(window *window) {
    double current_time = glfwGetTime();

    window->delta_time = current_time - window->previous_time;
    window->previous_time = current_time;
}

void window_set_framebuffer_size_callback(
    window *window, window_framebuffer_size_callback function) {
    window->framebuffer_size_callback = function;
}

void window_set_cursor_pos_callback(window *window,
                                    window_cursor_pos_callback function) {
    window->cursor_pos_callback = function;
}

void window_set_scroll_callback(window *window,
                                window_scroll_callback function) {
    window->scroll_callback = function;
}

// Update mouse buttons
void window_update_input(window *window) {
    keyboard_update_state(&window->keyboard);
    mouse_update_state(&window->mouse);

    for (int i = 0; i < KEYCODE_LAST; i++) {
        if (glfwGetKey(window->glfw_window, keycode_to_glfw(i)) == GLFW_PRESS) {
            keyboard_set_key(&window->keyboard, i, KEY_STATE_DOWN);
        }

        else if (glfwGetKey(window->glfw_window, keycode_to_glfw(i)) ==
                 GLFW_RELEASE) {
            keyboard_set_key(&window->keyboard, i, KEY_STATE_UP);
        }
    }

    for (int i = 0; i < MOUSE_BUTTON_LAST; i++) {
        if (glfwGetMouseButton(window->glfw_window, mouse_button_to_glfw(i)) ==
            GLFW_PRESS) {
            mouse_set_button(&window->mouse, i, BUTTON_STATE_DOWN);
        }

        else if (glfwGetMouseButton(window->glfw_window,
                                    mouse_button_to_glfw(i)) == GLFW_RELEASE) {
            mouse_set_button(&window->mouse, i, BUTTON_STATE_UP);
        }
    }

    double mouse_x;
    double mouse_y;

    glfwGetCursorPos(window->glfw_window, &mouse_x, &mouse_y);

    mouse_set_position(&window->mouse, (vector2d){mouse_x, mouse_y});

    glfwPollEvents(); // should this be at the start?
}
