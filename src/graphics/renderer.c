#include "renderer.h"

#include "GLFW/glfw3.h"
#include "gl_util.h"

void renderer_init() {
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    GL_CALL(glEnable(GL_CULL_FACE));
    GL_CALL(glCullFace(GL_BACK));
    GL_CALL(glFrontFace(GL_CCW));

    GL_CALL(glEnable(GL_DEPTH_TEST));
}

void renderer_clear_buffers() {
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void renderer_set_clear_colour(float red, float green, float blue,
                               float alpha) {
    GL_CALL(glClearColor(red, green, blue, alpha));
}

void renderer_set_viewport(int x, int y, int width, int height) {
    GL_CALL(glViewport(x, y, width, height));
}

GLenum to_gl_draw_mode(draw_mode mode) {
    switch (mode) {
    case DRAW_MODE_TRIANGLES:
        return GL_TRIANGLES;
    }
}

void renderer_draw_arrays(draw_mode mode, int first, int count) {
    GLenum gl_mode = to_gl_draw_mode(mode);
    GL_CALL(glDrawArrays(gl_mode, first, count));
}
