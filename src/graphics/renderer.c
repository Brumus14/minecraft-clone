#include "renderer.h"

#include "gl_util.h"
#include "GLFW/glfw3.h"

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

GLenum to_gl_index_type(index_type type) {
    switch (type) {
    case INDEX_TYPE_UNSIGNED_INT:
        return GL_UNSIGNED_INT;
    case INDEX_TYPE_UNSIGNED_SHORT:
        return GL_UNSIGNED_SHORT;
    case INDEX_TYPE_UNSIGNED_BYTE:
        return GL_UNSIGNED_BYTE;
    }
}

void renderer_draw_elements(draw_mode mode, int count, index_type type) {
    GL_CALL(glDrawElements(to_gl_draw_mode(mode), count, to_gl_index_type(type),
                           0));
}

GLenum to_gl_polygon_mode(polygon_mode mode) {
    switch (mode) {
    case POLYGON_MODE_FILL:
        return GL_FILL;
    case POLYGON_MODE_LINE:
        return GL_LINE;
    }
}

void renderer_set_polygon_mode(polygon_mode mode) {
    glPolygonMode(GL_FRONT_AND_BACK, to_gl_polygon_mode(mode));
}
