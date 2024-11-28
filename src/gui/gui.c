#include "gui.h"

#include "cglm/cglm.h"
#include "../graphics/gl_util.h"
#include "image.h"

// maybe batch render all gui

void gui_init(gui *gui, window *window) {
    gui->window = window;

    gui->element_count = 0;
    gui->elements = malloc(0);

    shader_program_from_files(&gui->shader_program, "res/shaders/gui.vert",
                              "res/shaders/gui.frag");
    shader_program_bind_attribute(&gui->shader_program, 0, "position");
    shader_program_link(&gui->shader_program);
}

void generate_orthographic_matrix(gui *gui) {
    mat4 glm_matrix;
    glm_ortho(0, gui->window->width, gui->window->height, 0, -1, 1, glm_matrix);

    gui->orthographic_matrix = matrix4d_from_glm(glm_matrix);
}

void gui_update_matrix_uniforms(gui *gui) {
    GLint shader_program_id;
    GL_CALL(glGetIntegerv(GL_CURRENT_PROGRAM, &shader_program_id));

    GLint orthographic_loc = GL_CALL_R(
        glGetUniformLocation(shader_program_id, "orthographic"), GLint);

    mat4 glm_orthographic = GLM_MAT4_IDENTITY_INIT;
    generate_orthographic_matrix(gui);
    matrix4d_to_glm(&gui->orthographic_matrix, glm_orthographic);

    GL_CALL(glUniformMatrix4fv(
        orthographic_loc, 1, GL_FALSE,
        (float *)glm_orthographic)); // reduce casting for performance
}

void gui_draw(gui *gui) {
    shader_program_use(&gui->shader_program);
    gui_update_matrix_uniforms(gui);

    for (int i = 0; i < gui->element_count; i++) {
        switch (gui->elements[i].type) {
        case GUI_ELEMENT_TYPE_IMAGE: {
            gui_image *image = gui->elements[i].data.image;

            if (!image->visible) {
                break;
            }

            texture_bind(&image->texture);
            vao_bind(&image->vao);
            bo_bind(&image->ibo);
            bo_bind(&image->vbo);

            int index_count = bo_get_size(&image->ibo) /
                              sizeof(unsigned int); // already know this value

            renderer_draw_elements(DRAW_MODE_TRIANGLES, index_count,
                                   INDEX_TYPE_UNSIGNED_INT);

            break;
        }
        }
    }
}

void gui_add_element(gui *gui, gui_element element) {
    gui->elements =
        realloc(gui->elements, (gui->element_count + 1) * sizeof(gui_element));
    gui->elements[gui->element_count] = element;
    gui->element_count++;
}

void gui_add_image(gui *gui, gui_image *image) {
    gui_element element;

    element.data.image = image;
    element.type = GUI_ELEMENT_TYPE_IMAGE;

    gui_add_element(gui, element);
}
