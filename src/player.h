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
    bool moved_this_frame;
    block_type selected_block;
} player;

void player_init(player *player, vector3d position, vector3d rotation,
                 double speed, double sensitivity, camera *camera);
void player_handle_input(player *player, window *window);
void player_manage_chunks(player *player, world *world);
bool player_get_target_block(player *player, world *world,
                             vector3d *position_dest, block_face *face);
void player_set_target_block(player *player, world *world, block_type type);
void player_destroy_block(player *player, world *world);
void player_place_block(player *player, world *world, block_type type);

#endif
