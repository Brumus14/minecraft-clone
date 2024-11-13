#include "player.h"

#include "math/math_util.h"

void player_init(player *player, vector3 position, vector3 rotation,
                 float speed, float sensitivity, camera *camera) {
    player->position = position;
    player->rotation = rotation;
    player->speed = speed;
    player->sensitivity = sensitivity;
    player->camera = camera;
}

void player_handle_input(player *player, window *window) {
    vector3 movement_delta;
    vector3_init(&movement_delta, 0.0, 0.0, 0.0);

    vector3 rotation_delta;
    vector3_init(&rotation_delta, 0.0, 0.0, 0.0);

    if (keyboard_key_down(&window->keyboard, KEYCODE_S)) {
        movement_delta.z += 1.0;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_W)) {
        movement_delta.z += -1.0;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_A)) {
        movement_delta.x += -1.0;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_D)) {
        movement_delta.x += 1.0;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_SPACE)) {
        movement_delta.y += 1.0;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_LEFT_SHIFT)) {
        movement_delta.y += -1.0;
    }

    rotation_delta.x = -window->mouse.position_delta.y * 0.1;
    rotation_delta.y = window->mouse.position_delta.x * 0.1;

    vector3_scalar_multiply_to(
        rotation_delta, player->sensitivity * window_get_delta_time(window),
        &rotation_delta);

    player->rotation = vector3_add(player->rotation, rotation_delta);

    // Make sure rotation is always below 360 degrees
    player->rotation.x = fmod(player->rotation.x + 360, 360);
    player->rotation.y = fmod(player->rotation.y + 360, 360);
    player->rotation.z = fmod(player->rotation.z + 360, 360);

    /*printf("%f, %f, %f\n", player->rotation.x, player->rotation.y,*/
    /*       player->rotation.z);*/

    vector3_normalise(&movement_delta);

    vector3 forwards = rotation_to_direction(player->rotation);
    vector3 right = vector3_cross_product(forwards, (vector3){0.0, 1.0, 0.0});

    forwards.y = 0;
    vector3_normalise(&forwards);

    right.y = 0;
    vector3_normalise(&right);

    vector3 relative_movement_delta;
    vector3_add_to(relative_movement_delta,
                   vector3_scalar_multiply(right, movement_delta.x),
                   &relative_movement_delta);
    vector3_add_to(relative_movement_delta,
                   vector3_scalar_multiply(forwards, -movement_delta.z),
                   &relative_movement_delta);
    relative_movement_delta.y = movement_delta.y;

    vector3_normalise(&relative_movement_delta);
    vector3_scalar_multiply_to(relative_movement_delta,
                               player->speed * window_get_delta_time(window),
                               &relative_movement_delta);

    vector3_add_to(player->position, relative_movement_delta,
                   &player->position);
}
