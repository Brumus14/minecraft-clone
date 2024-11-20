#include "camera.h"

#include "glad/glad.h"
#include "gl_util.h"
#include "../math/math_util.h"

void generate_perspective_matrix(camera *camera) {
    if (camera == NULL) {
        fprintf(stderr, "generate_perspective_matrix: camera is null\n");
        return;
    }

    glm_perspective(glm_rad(camera->fov), camera->aspect_ratio,
                    camera->near_plane, camera->far_plane,
                    camera->projection_matrix);
}

void generate_view_matrix(camera *camera) {
    if (camera == NULL) {
        fprintf(stderr, "generate_view_matrix: camera is null\n");
        return;
    }

    /*vector3d direction = direction_from_rotation(camera->rotation);*/
    vector3d direction = rotation_to_direction(camera->rotation);

    vec3 glm_direction;
    vector3d_to_glm(direction, &glm_direction);

    vec3 glm_position;
    vector3d_to_glm(camera->position, &glm_position);

    glm_look(glm_position, glm_direction, (vec3){0.0, 1.0, 0.0},
             camera->view_matrix);
}

void camera_init(camera *camera, vector3d position, vector3d rotation,
                 double fov, double aspect_ratio, double near_plane,
                 double far_plane) {
    if (camera == NULL) {
        fprintf(stderr, "camera_init: camera is null\n");
        return;
    }

    vec3 glm_position;
    vector3d_to_glm(position, &glm_position);

    camera->position = position;
    camera->rotation = rotation;
    camera->fov = fov;
    camera->near_plane = near_plane;
    camera->far_plane = far_plane;

    generate_view_matrix(camera);
    generate_perspective_matrix(camera);
}

void camera_set_position(camera *camera, vector3d position) {
    camera->position = position;
}

void camera_move(camera *camera, vector3d movement_delta) {
    if (camera == NULL) {
        fprintf(stderr, "camera_translate: camera is null\n");
        return;
    }

    /*vector3d direction = direction_from_rotation(camera->rotation);*/
    vector3d direction = rotation_to_direction(camera->rotation);

    vector3d position_delta;
    vector3d_init(&position_delta, 0.0, 0.0, 0.0);

    vector3d forwards = direction;
    forwards.y = 0.0;
    vector3d_normalise(&forwards);

    position_delta = vector3d_add(
        position_delta, vector3d_scalar_multiply(forwards, -movement_delta.z));

    vector3d up;
    vector3d_init(&up, 0.0, 1.0, 0.0);

    position_delta = vector3d_add(
        position_delta,
        vector3d_scalar_multiply(
            vector3d_normalised(vector3d_cross_product(direction, up)),
            movement_delta.x));

    position_delta.y += movement_delta.y;

    camera->position = vector3d_add(camera->position, position_delta);

    generate_view_matrix(camera);
}

void camera_update_matrix_uniforms(camera *camera) {
    if (camera == NULL) {
        fprintf(stderr, "camera_update_matrix_uniforms: camera is null\n");
        return;
    }

    GLint shader_program_id;
    GL_CALL(glGetIntegerv(GL_CURRENT_PROGRAM, &shader_program_id));

    GLint view_loc =
        GL_CALL_R(glGetUniformLocation(shader_program_id, "view"), GLint);

    GL_CALL(glUniformMatrix4fv(
        view_loc, 1, GL_FALSE,
        (float *)camera->view_matrix)); // reduce casting for performance

    GLint projection_loc =
        GL_CALL_R(glGetUniformLocation(shader_program_id, "projection"), GLint);

    GL_CALL(glUniformMatrix4fv(projection_loc, 1, GL_FALSE,
                               (float *)camera->projection_matrix));

    // should this be moved somewhere else?
    mat4 model_matrix = GLM_MAT4_IDENTITY;
    GLint model_loc = GL_CALL(glGetUniformLocation(shader_program_id, "model"));
    GL_CALL(glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float *)model_matrix));
}

void camera_set_aspect_ratio(camera *camera, double aspect_ratio) {
    if (camera == NULL) {
        fprintf(stderr, "camera_set_aspect_ratio: camera is null\n");
        return;
    }

    camera->aspect_ratio = aspect_ratio;

    generate_perspective_matrix(camera);
}

void camera_set_rotation(camera *camera, vector3d rotation) {
    if (camera == NULL) {
        fprintf(stderr, "camera_set_rotation: camera is null\n");
        return;
    }

    camera->rotation = rotation;

    generate_view_matrix(camera);
}

void camera_rotate(camera *camera, vector3d rotation_delta) {
    if (camera == NULL) {
        fprintf(stderr, "camera_rotate: camera is null\n");
        return;
    }

    vector3d new_rotation = vector3d_add(camera->rotation, rotation_delta);

    camera_set_rotation(camera, new_rotation);
}
