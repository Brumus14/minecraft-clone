#ifndef CAMERA_H
#define CAMERA_H

#include "cglm/cglm.h"
#include "cglm/cam.h"

typedef struct camera {
    vec3 position;
    vec3 rotation;
    float fov;
    float aspect_ratio;
    float near_plane;
    float far_plane;
    mat4 view_matrix;
    mat4 projection_matrix;
} camera;

void camera_init(camera *camera, vec3 position, vec3 rotation, float fov,
                 float aspect_ratio, float near_plane, float far_plane);
void camera_translate(camera *camera, vec3 translation);
void camera_update_matrices(camera *camera);
void camera_set_aspect_ratio(camera *camera, float aspect_ratio);

#endif
