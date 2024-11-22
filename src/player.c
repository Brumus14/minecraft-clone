#include "player.h"

#include <stdlib.h>
#include "math/math_util.h"

void player_init(player *player, vector3d position, vector3d rotation,
                 double speed, double sensitivity, camera *camera) {
    player->position = position;
    player->rotation = rotation;
    player->speed = speed;
    player->sensitivity = sensitivity;
    player->camera = camera;
}

void player_handle_input(player *player, window *window) {
    vector3d movement_delta;
    vector3d_init(&movement_delta, 0.0, 0.0, 0.0);

    vector3d rotation_delta;
    vector3d_init(&rotation_delta, 0.0, 0.0, 0.0);

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

    double delta_time = window_get_delta_time(window);

    rotation_delta.x = -window->mouse.position_delta.y;
    rotation_delta.y = window->mouse.position_delta.x;

    vector3d_scalar_multiply_to(rotation_delta, player->sensitivity,
                                &rotation_delta);

    vector3d_add_to(player->rotation, rotation_delta, &player->rotation);

    player->rotation.x = clamp(player->rotation.x, -89.9, 89.9);

    // Make sure rotation is always below 360 degrees or maybe -180 and 180
    player->rotation.y = fmod(player->rotation.y + 360, 360);

    vector3d_normalise(&movement_delta);

    vector3d forwards = rotation_to_direction(player->rotation);
    vector3d right =
        vector3d_cross_product(forwards, (vector3d){0.0, 1.0, 0.0});

    forwards.y = 0;
    vector3d_normalise(&forwards);

    right.y = 0;
    vector3d_normalise(&right);

    vector3d relative_movement_delta = VECTOR3D_ZERO;
    vector3d_add_to(relative_movement_delta,
                    vector3d_scalar_multiply(right, movement_delta.x),
                    &relative_movement_delta);
    vector3d_add_to(relative_movement_delta,
                    vector3d_scalar_multiply(forwards, -movement_delta.z),
                    &relative_movement_delta);
    relative_movement_delta.y = movement_delta.y;

    vector3d_normalise(&relative_movement_delta);
    vector3d_scalar_multiply_to(relative_movement_delta,
                                player->speed * delta_time,
                                &relative_movement_delta);

    vector3d_add_to(player->position, relative_movement_delta,
                    &player->position);

    // maybe not do this???
    player->moved_this_frame =
        vector3d_magnitude(relative_movement_delta) > EPSILON;
}

void player_manage_chunks(player *player, world *world) {
    /*if (!player->moved_this_frame) {*/
    /*    return;*/
    /*}*/

    int render_distance = 2; // move to a variable
    vector2i player_chunk;
    player_chunk.x = floor(player->position.x / CHUNK_SIZE_X);
    player_chunk.y = floor(player->position.z / CHUNK_SIZE_Z);

    vector3i unloaded_chunks[world->chunk_count]; // DONT USE THIS
    int unloaded_chunk_count = 0;

    for (int i = 0; i < world->chunk_count; i++) {
        chunk *chunk = &world->chunks[i];

        if (chunk->position.x < player_chunk.x - render_distance ||
            chunk->position.x > player_chunk.x + render_distance ||
            chunk->position.z < player_chunk.y - render_distance ||
            chunk->position.z > player_chunk.y + render_distance) {
            unloaded_chunks[unloaded_chunk_count] = chunk->position;
            unloaded_chunk_count++;
        }
    }

    for (int i = 0; i < unloaded_chunk_count; i++) {
        world_unload_chunk(world, unloaded_chunks[i]);
    }

    for (int y = -render_distance; y <= render_distance; y++) {
        for (int x = -render_distance; x <= render_distance; x++) {
            world_load_chunk(
                world, (vector3i){player_chunk.x + x, -4, player_chunk.y + y});
            world_load_chunk(
                world, (vector3i){player_chunk.x + x, -3, player_chunk.y + y});
            world_load_chunk(
                world, (vector3i){player_chunk.x + x, -2, player_chunk.y + y});
            world_load_chunk(
                world, (vector3i){player_chunk.x + x, -1, player_chunk.y + y});
            world_load_chunk(
                world, (vector3i){player_chunk.x + x, 0, player_chunk.y + y});
            world_load_chunk(
                world, (vector3i){player_chunk.x + x, 1, player_chunk.y + y});
            world_load_chunk(
                world, (vector3i){player_chunk.x + x, 2, player_chunk.y + y});
        }
    }
}

void player_get_target_block(player *player, world *world) {
    float max_ray_length = 5;

    vector3d ray_origin = player->position;
    vector3d direction = rotation_to_direction(player->rotation);
    vector3i voxel_origin = {floor(ray_origin.x), floor(ray_origin.y),
                             floor(ray_origin.z)};
    // when direction 0
    vector3d steps = {direction.x > 0 ? 1 : -1, direction.y > 0 ? 1 : -1,
                      direction.z > 0 ? 1 : -1};
    vector3d deltas = {1 / direction.x, 1 / direction.y, 1 / direction.z};

    float travelled = 0;
    bool hit = false;
}
