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

// if pointers are null
bool player_get_target_block(player *player, world *world,
                             vector3d *position_dest, block_face *face) {
    float max_ray_length = 5; // move to variable

    vector3d ray_origin = player->position;

    if (world_get_block(world, ray_origin) != BLOCK_TYPE_EMPTY) {
        *position_dest = ray_origin;

        if (face != NULL) {
            *face = -1; // this is bit sus
        }

        return true;
    }

    // when direction is 0
    vector3d direction = rotation_to_direction(player->rotation);

    if (direction.x == 0) {
        direction.x = EPSILON; // use separate epsilon
    }

    if (direction.y == 0) {
        direction.y = EPSILON;
    }

    if (direction.z == 0) {
        direction.z = EPSILON;
    }

    vector3i voxel_position = {floor(ray_origin.x), floor(ray_origin.y),
                               floor(ray_origin.z)};

    vector3d delta_distance = {fabs(1 / direction.x), fabs(1 / direction.y),
                               fabs(1 / direction.z)};

    vector3i step = VECTOR3I_ZERO;
    vector3d side_distance = VECTOR3D_ZERO;
    block_face current_face;
    bool hit = false;

    if (direction.x < 0) {
        step.x = -1;
        side_distance.x = (ray_origin.x - voxel_position.x) * delta_distance.x;
    } else {
        step.x = 1;
        side_distance.x =
            (voxel_position.x + 1 - ray_origin.x) * delta_distance.x;
    }

    if (direction.y < 0) {
        step.y = -1;
        side_distance.y = (ray_origin.y - voxel_position.y) * delta_distance.y;
    } else {
        step.y = 1;
        side_distance.y =
            (voxel_position.y + 1 - ray_origin.y) * delta_distance.y;
    }

    if (direction.z < 0) {
        step.z = -1;
        side_distance.z = (ray_origin.z - voxel_position.z) * delta_distance.z;
    } else {
        step.z = 1;
        side_distance.z =
            (voxel_position.z + 1 - ray_origin.z) * delta_distance.z;
    }

    while (!hit && (side_distance.x < max_ray_length ||
                    side_distance.y < max_ray_length ||
                    side_distance.z < max_ray_length)) {
        if (side_distance.x < side_distance.y &&
            side_distance.x < side_distance.z) {
            side_distance.x += delta_distance.x;
            voxel_position.x += step.x;

            current_face = step.x > 0 ? BLOCK_FACE_LEFT : BLOCK_FACE_RIGHT;
        } else if (side_distance.y < side_distance.z) {
            side_distance.y += delta_distance.y;
            voxel_position.y += step.y;

            current_face = step.y > 0 ? BLOCK_FACE_BOTTOM : BLOCK_FACE_TOP;
        } else {
            side_distance.z += delta_distance.z;
            voxel_position.z += step.z;

            current_face = step.z > 0 ? BLOCK_FACE_BACK : BLOCK_FACE_FRONT;
        }

        vector3d position =
            (vector3d){voxel_position.x, voxel_position.y, voxel_position.z};

        if (world_get_block(world, position)) {
            hit = true;

            *position_dest = position;

            if (face != NULL) {
                *face = current_face;
            }

            return true;
        }
    }

    return false;
}

void player_set_target_block(player *player, world *world, block_type type) {
    vector3d target_block;

    if (player_get_target_block(player, world, &target_block, NULL)) {
        world_set_block(world, type, target_block);
    }
}

void player_destroy_block(player *player, world *world) {
    player_set_target_block(player, world, BLOCK_TYPE_EMPTY);
}

void player_place_block(player *player, world *world, block_type type) {
    vector3d target_block;
    block_face target_face;

    if (!player_get_target_block(player, world, &target_block, &target_face)) {
        return;
    }

    if (target_face == -1) {
        return;
    }

    vector3d block_position = target_block;

    switch (target_face) {
    case BLOCK_FACE_FRONT:
        block_position.z++;
        break;
    case BLOCK_FACE_TOP:
        block_position.y++;
        break;
    case BLOCK_FACE_RIGHT:
        block_position.x++;
        break;
    case BLOCK_FACE_BOTTOM:
        block_position.y--;
        break;
    case BLOCK_FACE_LEFT:
        block_position.x--;
        break;
    case BLOCK_FACE_BACK:
        block_position.z--;
        break;
    }

    world_set_block(world, type, block_position);
}
