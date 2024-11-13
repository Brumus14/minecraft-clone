#ifndef PLAYER_H
#define PLAYER_H

#include "math/vector3.h"
#include "graphics/camera.h"
#include "graphics/window.h"

typedef struct player {
    vector3 position;
    vector3 rotation;
    camera *camera;
    float speed;
    float sensitivity;
} player;

void player_init(player *player, vector3 position, vector3 rotation,
                 float speed, float sensitivity, camera *camera);
void player_handle_input(player *player, window *window);

#endif
