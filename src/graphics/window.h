#ifndef WINDOW_H
#define WINDOW_H

#include "renderer.h"
#include "GLFW/glfw3.h"
#include <stdbool.h>
#include "camera.h"
#include "../input/keyboard.h"
#include "../input/mouse.h"

typedef struct window window;

typedef void (*window_framebuffer_size_callback)(window *, int, int);
typedef void (*window_cursor_pos_callback)(window *, double, double);
typedef void (*window_scroll_callback)(window *, double, double);

typedef struct window {
    GLFWwindow *glfw_window;
    int width;
    int height;
    char *title;
    camera *camera;
    keyboard keyboard;
    mouse mouse;
    double delta_time;
    double previous_time;
    window_framebuffer_size_callback framebuffer_size_callback;
    window_cursor_pos_callback cursor_pos_callback;
    window_scroll_callback scroll_callback;
} window;

void glfw_init();
void window_init(window *window, int width, int height, char *title,
                 camera *camera);
bool window_should_close(window *window);
void window_swap_buffers(window *window);
void window_destroy(window *window);
float window_get_aspect_ratio(window *window);
void window_reset_cursor(window *window);
void window_capture_cursor(window *window);
void window_set_framebuffer_size_callback(
    window *window, window_framebuffer_size_callback function);
void window_set_cursor_pos_callback(window *window,
                                    window_cursor_pos_callback function);
void window_set_scroll_callback(window *window,
                                window_scroll_callback function);
double window_get_delta_time(window *window);
void window_update_delta_time(window *window);
void window_update_input(window *window);

#endif
