#include "gui.h"

#include "cglm/cglm.h"
#include "../graphics/gl_util.h"

void gui_init(gui *gui, window *window) {
    gui->window = window;

    shader_program_from_files(&gui->shader_program, "res/shaders/gui.vert",
                              "res/shaders/gui.frag");
    shader_program_bind_attribute(&gui->shader_program, 0, "position");
    shader_program_link(&gui->shader_program);
}

void generate_orthographic_matrix(gui *gui) {
    mat4 glm_matrix;
    glm_ortho_default((float)gui->window->width / gui->window->height,
                      glm_matrix);

    gui->orthographic_matrix = matrix4d_from_glm(glm_matrix);
}

void gui_update_matrix_uniforms(gui *gui) {
    GLint shader_program_id;
    GL_CALL(glGetIntegerv(GL_CURRENT_PROGRAM, &shader_program_id));

    GLint orthographic_loc = GL_CALL_R(
        glGetUniformLocation(shader_program_id, "orthographic"), GLint);

    mat4 glm_orthographic = GLM_MAT4_IDENTITY_INIT;
    glm_ortho_default((float)gui->window->width / gui->window->height,
                      glm_orthographic);
    /*matrix4d_to_glm(&gui->orthographic_matrix, glm_orthographic);*/

    GL_CALL(glUniformMatrix4fv(
        orthographic_loc, 1, GL_FALSE,
        (float *)glm_orthographic)); // reduce casting for performance
}

void gui_prepare_draw(gui *gui) {
    shader_program_use(&gui->shader_program);
    gui_update_matrix_uniforms(gui);
}
