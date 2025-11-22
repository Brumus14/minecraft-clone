#ifndef PLAYER_H
#define PLAYER_H

#include "../math/vector3.h"
#include "../graphics/camera.h"
#include "../graphics/window.h"
#include "../world/world.h"

#define DEFAULT_SPEED 10.92
#define SPRINTING_SPEED 21.6

#define COLLISION_BOX_X 0.6
#define COLLISION_BOX_Y 1.8
#define COLLISION_BOX_Z 0.6

typedef struct player {
    vector3d position;
    vector3d rotation;
    camera *camera;
    double acceleration;
    double deceleration;
    double speed;
    vector3d velocity;
    double sensitivity;
    bool sprinting;
} player;

void player_init(player *player, vector3d position, vector3d rotation,
                 double sensitivity, camera *camera);
void player_handle_input(player *player, window *window, world *world);
void player_update(player *player, window *window, world *world);
void player_manage_chunks(player *player, world *world);
bool player_get_target_block(player *player, world *world,
                             vector3d *position_dest, block_face *face);
void player_set_target_block(player *player, world *world, block_type type);
void player_destroy_block(player *player, world *world);
void player_place_block(player *player, world *world, block_type type);
void player_replace_block(player *player, world *world, block_type type);

#endif
