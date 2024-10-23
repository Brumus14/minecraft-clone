#include "camera.h"

#include "glad/glad.h"
#include "cglm/io.h"

vector3 direction_from_rotation(vector3 rotation);

void camera_init(camera *camera, vector3 position, vector3 rotation, float fov,
                 float aspect_ratio, float near_plane, float far_plane) {
    vec3 glm_position;
    vector3_to_glm(position, &glm_position);

    camera->position = position;
    camera->rotation = rotation;

    glm_look(glm_position, (vec3){0.0, 0.0, 0.0}, (vec3){0.0, 1.0, 0.0},
             camera->view_matrix);

    camera->fov = fov;
    camera->near_plane = near_plane;
    camera->far_plane = far_plane;
    glm_perspective(fov, aspect_ratio, near_plane, far_plane,
                    camera->projection_matrix);
}

void camera_translate(camera *camera, vector3 translation) {
    vector3 direction = direction_from_rotation(camera->rotation);

    vector3 position_delta;
    vector3_init(&position_delta, 0.0, 0.0, 0.0);

    vector3 forwards = direction;
    forwards.y = 0.0;
    vector3_normalise(&forwards);

    position_delta = vector3_add(
        position_delta, vector3_scalar_multiply(forwards, -translation.z));

    vector3 up;
    vector3_init(&up, 0.0, 1.0, 0.0);

    position_delta = vector3_add(
        position_delta,
        vector3_scalar_multiply(
            vector3_normalised(vector3_cross_product(direction, up)),
            translation.x));

    position_delta.y += translation.y;

    camera->position = vector3_add(camera->position, position_delta);

    vec3 glm_direction;
    vector3_to_glm(direction, &glm_direction);

    glm_vec3_print(glm_direction, stdout);

    vec3 glm_position;
    vector3_to_glm(camera->position, &glm_position);

    glm_look(glm_position, glm_direction, (vec3){0.0, 1.0, 0.0},
             camera->view_matrix);
}

// rename function
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

void camera_set_aspect_ratio(camera *camera, float aspect_ratio) {
    camera->aspect_ratio = aspect_ratio;

    // move into update perspective matrix function
    glm_perspective(camera->fov, aspect_ratio, camera->near_plane,
                    camera->far_plane, camera->projection_matrix);

    /*camera_update_matrices(camera);*/
}

void camera_set_rotation(camera *camera, vector3 rotation) {
    vec3 glm_rotation;
    vector3_to_glm(rotation, &glm_rotation);

    camera->rotation = rotation;

    // make rotation to direction function
    vector3 direction = direction_from_rotation(rotation);

    vec3 glm_direction;
    vector3_to_glm(direction, &glm_direction);

    vec3 glm_position;
    vector3_to_glm(camera->position, &glm_position);

    glm_look(glm_position, glm_direction, (vec3){0.0, 1.0, 0.0},
             camera->view_matrix);

    glm_mat4_print(camera->view_matrix, stdout);

    /*camera_update_matrices(camera);*/
}

void camera_rotate(camera *camera, vector3 delta_rotation) {
    vector3 new_rotation = vector3_add(camera->rotation, delta_rotation);

    camera_set_rotation(camera, new_rotation);
}

vector3 direction_from_rotation(vector3 rotation) {
    vector3 direction;

    direction.x = cos(glm_rad(rotation.y)) * cos(glm_rad(rotation.x));
    direction.y = sin(glm_rad(rotation.x));
    direction.z = sin(glm_rad(rotation.y)) * cos(glm_rad(rotation.x));

    vector3_normalise(&direction);

    return direction;
}
