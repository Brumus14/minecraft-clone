#include "camera.h"

#include "glad/glad.h"

void camera_init(camera *camera, vec3 position, vec3 rotation, float fov,
                 float near_plane, float far_plane) {
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy(rotation, camera->rotation);
    glm_lookat(position, (vec3){0.0, 0.0, 0.0}, (vec3){0.0, 1.0, 0.0},
               camera->view_matrix);

    camera->fov = fov;
    camera->near_plane = near_plane;
    camera->far_plane = far_plane;
    glm_perspective(fov, 16.0 / 9.0, near_plane, far_plane,
                    camera->projection_matrix);
}

void camera_translate(camera *camera, vec3 translation) {
    glm_vec3_add(camera->position, translation, camera->position);
    glm_lookat(camera->position, (vec3){0.0, 0.0, 0.0}, (vec3){0.0, 1.0, 0.0},
               camera->view_matrix);
}

void camera_update_matrices(camera *camera) {
    GLint shader_program_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shader_program_id);

    GLint view_loc = glGetUniformLocation(shader_program_id, "view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float *)camera->view_matrix);

    GLint projection_loc =
        glGetUniformLocation(shader_program_id, "projection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE,
                       (float *)camera->projection_matrix);
}
