#ifndef CAMERA_H
#define CAMERA_H

#include "../math/vector3.h"

// create custom matrix4 implementation
typedef struct camera {
    vector3d position;
    vector3d rotation;
    double fov;
    double aspect_ratio;
    double near_plane;
    double far_plane;
    mat4 view_matrix;
    mat4 projection_matrix;
} camera;

void camera_init(camera *camera, vector3d position, vector3d rotation,
                 double fov, double aspect_ratio, double near_plane,
                 double far_plane);
void camera_set_position(camera *camera, vector3d position);
void camera_move(camera *camera, vector3d movement_delta);
void camera_update_matrix_uniforms(camera *camera);
void camera_set_aspect_ratio(camera *camera, double aspect_ratio);
void camera_set_rotation(camera *camera, vector3d rotation);
void camera_rotate(camera *camera, vector3d rotation_delta);

#endif
