#ifndef PLAYER_H
#define PLAYER_H

#include "math/vector3.h"
#include "graphics/camera.h"
#include "graphics/window.h"
#include "world/world.h"

typedef struct player {
    vector3d position;
    vector3d rotation;
    camera *camera;
    double speed;
    double sensitivity;
} player;

void player_init(player *player, vector3d position, vector3d rotation,
                 double speed, double sensitivity, camera *camera);
void player_handle_input(player *player, window *window);
void player_manage_chunks(player *player, world *world);

#endif
