#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef struct window {
    GLFWwindow *glfw_window;
    int width;
    int height;
    char *title;
} window;

void init_glfw();
void init_window(window *window, int width, int height, char *title);
bool window_should_close(window *window);
void window_swap_buffers(window *window);
void window_destroy(window *window);
float window_get_aspect_ratio(window *window);
