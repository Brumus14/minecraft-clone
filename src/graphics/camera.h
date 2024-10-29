#ifndef CAMERA_H
#define CAMERA_H

#include "../math/vector3.h"

typedef struct camera {
    vector3 position;
    vector3 rotation;
    float fov;
    float aspect_ratio;
    float near_plane;
    float far_plane;
    mat4 view_matrix;
    mat4 projection_matrix;
} camera;

void camera_init(camera *camera, vector3 position, vector3 rotation, float fov,
                 float aspect_ratio, float near_plane, float far_plane);
void camera_translate(camera *camera, vector3 translation);
void camera_update_matrix_uniforms(camera *camera);
void camera_set_aspect_ratio(camera *camera, float aspect_ratio);
void camera_set_rotation(camera *camera, vector3 rotation);
void camera_rotate(camera *camera, vector3 delta_rotation);

#endif
