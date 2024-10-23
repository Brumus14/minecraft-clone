#include "renderer.h"

#include "GLFW/glfw3.h"

void init_renderer() {
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void clear_colour() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void set_clear_colour(float red, float green, float blue, float alpha) {
    glClearColor(red, green, blue, alpha);
}

void set_viewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

GLenum to_gl_draw_mode(draw_mode mode) {
    switch (mode) {
    case TRIANGLES:
        return GL_TRIANGLES;
    }
}

void draw_arrays(draw_mode mode, int first, int count) {
    GLenum gl_mode = to_gl_draw_mode(mode);
    glDrawArrays(gl_mode, first, count);
}
