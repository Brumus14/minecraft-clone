#include "player.h"

#include <stdlib.h>
#include "math/math_util.h"
#include "collision.h"

void player_init(player *player, vector3d position, vector3d rotation,
                 double sensitivity, camera *camera) {
    player->position = position;
    player->rotation = rotation;
    player->speed = DEFAULT_SPEED;
    player->acceleration = player->speed * 4;
    player->velocity = VECTOR3D_ZERO;
    player->sensitivity = sensitivity;
    player->camera = camera;
    player->sprinting = false;
}

// move to AABB collision function
bool valid_position(world *world, vector3d position) {
    for (int z = -1; z <= 1; z++) {
        for (int y = -1; y <= 1; y++) {
            for (int x = -1; x <= 1; x++) {
                if (z == 0 && y == 0 && x == 0) {
                    continue;
                }

                if (world_get_block(
                        world, vector3d_add(position, (vector3d){x, y, z})) ==
                    BLOCK_TYPE_EMPTY) {
                    continue;
                }

                cuboid voxel_cuboid;
                cuboid_init(&voxel_cuboid, floor(position.x + x),
                            floor(position.y + y), floor(position.z + z), 1, 1,
                            1);

                cuboid player_cuboid;
                cuboid_init(&player_cuboid, position.x - (COLLISION_BOX_X / 2),
                            position.y - (COLLISION_BOX_Y / 2),
                            position.z - (COLLISION_BOX_Z / 2), COLLISION_BOX_X,
                            COLLISION_BOX_Y, COLLISION_BOX_Z);

                if (collision_aabb_3d(player_cuboid, voxel_cuboid)) {
                    return false;
                }
            }
        }
    }

    return true;
}

void player_handle_input(player *player, window *window, world *world) {
    vector3d velocity_delta = VECTOR3D_ZERO;
    vector3d rotation_delta = VECTOR3D_ZERO;

    if (keyboard_key_down(&window->keyboard, KEYCODE_S)) {
        velocity_delta.z += 1;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_W)) {
        velocity_delta.z -= 1;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_A)) {
        velocity_delta.x -= 1;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_D)) {
        velocity_delta.x += 1;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_SPACE)) {
        velocity_delta.y += 1;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_LEFT_SHIFT)) {
        velocity_delta.y -= 1;
    }

    double delta_time = window_get_delta_time(window);

    // Update the rotation
    rotation_delta.x = -window->mouse.position_delta.y;
    rotation_delta.y = window->mouse.position_delta.x;

    vector3d_scalar_multiply_to(rotation_delta, player->sensitivity,
                                &rotation_delta);

    vector3d_add_to(player->rotation, rotation_delta, &player->rotation);

    player->rotation.x = clamp(player->rotation.x, -89.9, 89.9);
    player->rotation.y = fmod(player->rotation.y + 360, 360);

    // Update the position
    // make sure cant build up speed if cant move e.g. next to block edge
    if (vector3d_magnitude(velocity_delta) == 0) {
        player->sprinting = false;
    }

    player->speed = player->sprinting ? SPRINTING_SPEED : DEFAULT_SPEED;

    vector3d_normalise(&velocity_delta);
    vector3d_scalar_multiply_to(
        velocity_delta, player->acceleration * delta_time, &velocity_delta);

    vector3d_add_to(player->velocity, velocity_delta, &player->velocity);

    float decceleration = 20;

    if (velocity_delta.x == 0) {
        player->velocity.x +=
            -sign(player->velocity.x) *
            min(decceleration * delta_time, fabs(player->velocity.x));
    }

    if (velocity_delta.y == 0) {
        player->velocity.y +=
            -sign(player->velocity.y) *
            min(decceleration * delta_time, fabs(player->velocity.y));
    }

    if (velocity_delta.z == 0) {
        player->velocity.z +=
            -sign(player->velocity.z) *
            min(decceleration * delta_time, fabs(player->velocity.z));
    }

    if (vector3d_magnitude(player->velocity) > player->speed) {
        vector3d_normalise(&player->velocity);
        vector3d_scalar_multiply_to(player->velocity, player->speed,
                                    &player->velocity);
    }

    vector3d up = (vector3d){0, 1, 0};
    vector3d forwards = rotation_to_direction(player->rotation);
    vector3d right = vector3d_cross_product(forwards, up);

    forwards.y = 0;
    vector3d_normalise(&forwards);

    right.y = 0;
    vector3d_normalise(&right);

    vector3d position_delta =
        vector3d_scalar_multiply(player->velocity, delta_time);
    vector3d relative_position_delta = VECTOR3D_ZERO;

    vector3d_add_to(relative_position_delta,
                    vector3d_scalar_multiply(right, position_delta.x),
                    &relative_position_delta);

    vector3d_add_to(relative_position_delta,
                    vector3d_scalar_multiply(forwards, -position_delta.z),
                    &relative_position_delta);

    relative_position_delta.y = position_delta.y;

    vector3d_normalise(&relative_position_delta);
    vector3d_scalar_multiply_to(relative_position_delta,
                                vector3d_magnitude(position_delta),
                                &relative_position_delta);

    vector3d new_position =
        vector3d_add(player->position, relative_position_delta);

    // move position to block edge
    if (relative_position_delta.x < 0) {
        if (valid_position(world, (vector3d){new_position.x, player->position.y,
                                             player->position.z}) !=
            BLOCK_TYPE_EMPTY) {
            player->position.x = new_position.x;
        }
    } else if (relative_position_delta.x > 0) {
        if (valid_position(world, (vector3d){new_position.x, player->position.y,
                                             player->position.z}) !=
            BLOCK_TYPE_EMPTY) {
            player->position.x = new_position.x;
        }
    }

    if (relative_position_delta.y < 0) {
        if (valid_position(world, (vector3d){player->position.x, new_position.y,
                                             player->position.z}) !=
            BLOCK_TYPE_EMPTY) {
            player->position.y = new_position.y;
        }
    } else if (relative_position_delta.y > 0) {
        if (valid_position(world, (vector3d){player->position.x, new_position.y,
                                             player->position.z}) !=
            BLOCK_TYPE_EMPTY) {
            player->position.y = new_position.y;
        }
    }

    if (relative_position_delta.z < 0) {
        if (valid_position(world,
                           (vector3d){player->position.x, player->position.y,
                                      new_position.z}) != BLOCK_TYPE_EMPTY) {
            player->position.z = new_position.z;
        }
    } else if (relative_position_delta.z > 0) {
        if (valid_position(world,
                           (vector3d){player->position.x, player->position.y,
                                      new_position.z}) != BLOCK_TYPE_EMPTY) {
            player->position.z = new_position.z;
        }
    }
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

    vector3d ray_origin = player->camera->position;

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

void player_replace_block(player *player, world *world, block_type type) {
    vector3d target_block;

    if (!player_get_target_block(player, world, &target_block, NULL)) {
        return;
    }

    world_set_block(world, type, target_block);
}
