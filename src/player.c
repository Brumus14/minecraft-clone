#include "player.h"

#include <stdlib.h>
#include "math/math_util.h"
#include "collision.h"
#include "math/cuboid.h"
#include "util/gl.h"

void player_init(player *player, vector3d position, vector3d rotation,
                 double sensitivity, camera *camera) {
    player->position = position;
    player->rotation = rotation;
    player->speed = DEFAULT_SPEED;
    player->acceleration = player->speed * 8;
    player->velocity = VECTOR3D_ZERO;
    player->sensitivity = sensitivity;
    player->camera = camera;
    player->sprinting = false;
}

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

                // function to get this or store as variable
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

void player_update_rotation(player *player, window *window) {
    vector3d rotation_delta = (vector3d){-window->mouse.position_delta.y,
                                         window->mouse.position_delta.x, 0};

    vector3d_scalar_multiply_to(rotation_delta, player->sensitivity,
                                &rotation_delta);

    vector3d_add_to(player->rotation, rotation_delta, &player->rotation);

    // Clamp up and down rotation, and keep y rotation always between 0, 360
    player->rotation.x = clamp(player->rotation.x, -89.9, 89.9);
    player->rotation.y = fmod(player->rotation.y + 360, 360);
}

void player_update_movement(player *player, window *window) {
    double delta_time = window_get_delta_time(window);

    vector3d relative_velocity_delta = VECTOR3D_ZERO;
    vector3d velocity_delta = VECTOR3D_ZERO;

    // Calculate the relative velocity
    if (keyboard_key_down(&window->keyboard, KEYCODE_S)) {
        relative_velocity_delta.z += 1;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_W)) {
        relative_velocity_delta.z -= 1;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_A)) {
        relative_velocity_delta.x -= 1;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_D)) {
        relative_velocity_delta.x += 1;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_SPACE)) {
        relative_velocity_delta.y += 1;
    }

    if (keyboard_key_down(&window->keyboard, KEYCODE_LEFT_SHIFT)) {
        relative_velocity_delta.y -= 1;
    }

    if (vector3d_magnitude(relative_velocity_delta) == 0) {
        player->sprinting = false;
    }

    // Convert relative velocity delta to global velocity delta
    vector3d up = (vector3d){0, 1, 0};
    vector3d forwards =
        vector3d_scalar_multiply(rotation_to_direction(player->rotation), -1);

    // Remove forwards vertical component
    forwards.y = 0;
    vector3d_normalise(&forwards);

    vector3d right = vector3d_cross_product(up, forwards);

    vector3d_add_to(velocity_delta,
                    vector3d_scalar_multiply(up, relative_velocity_delta.y),
                    &velocity_delta);

    vector3d_add_to(
        velocity_delta,
        vector3d_scalar_multiply(forwards, relative_velocity_delta.z),
        &velocity_delta);

    vector3d_add_to(velocity_delta,
                    vector3d_scalar_multiply(right, relative_velocity_delta.x),
                    &velocity_delta);

    vector3d_normalise(&velocity_delta);

    vector3d_scalar_multiply_to(velocity_delta, delta_time * 2,
                                &velocity_delta);

    vector3d_add_to(player->velocity, velocity_delta, &player->velocity);

    vector3d_scalar_multiply_to(player->velocity, pow(0.2, delta_time),
                                &player->velocity);

    vector3d_add_to(
        player->position,
        vector3d_scalar_multiply(player->velocity, delta_time * player->speed),
        &player->position);
}

void player_update(player *player, window *window, world *world) {
    player_update_rotation(player, window);
    player_update_movement(player, window);
    player_manage_chunks(player, world);
}

void player_manage_chunks(player *player, world *world) {
    /*if (!player->moved_this_frame) {*/
    /*    return;*/
    /*}*/

    int render_distance = 2; // move to a variable
    vector3i player_chunk;
    player_chunk.x = floor(player->position.x / CHUNK_SIZE_X);
    player_chunk.y = floor(player->position.y / CHUNK_SIZE_Y);
    player_chunk.z = floor(player->position.z / CHUNK_SIZE_Z);

    vector3i unloaded_chunks[safe_linked_list_length(
        &world->chunks)]; // DONT USE THIS
    int unloaded_chunk_count = 0;

    for (int i = 0; i < safe_linked_list_length(&world->chunks); i++) {
        chunk *chunk = safe_linked_list_get(&world->chunks, i);

        if (chunk->position.x < player_chunk.x - render_distance ||
            chunk->position.x > player_chunk.x + render_distance ||
            chunk->position.y < player_chunk.y - render_distance ||
            chunk->position.y > player_chunk.y + render_distance ||
            chunk->position.z < player_chunk.z - render_distance ||
            chunk->position.z > player_chunk.z + render_distance) {
            unloaded_chunks[unloaded_chunk_count] = chunk->position;
            unloaded_chunk_count++;
        }
    }

    for (int i = 0; i < unloaded_chunk_count; i++) {
        world_unload_chunk(world, unloaded_chunks[i]);
    }

    for (int z = -render_distance; z <= render_distance; z++) {
        for (int y = -render_distance; y <= render_distance; y++) {
            for (int x = -render_distance; x <= render_distance; x++) {
                world_load_chunk(world, (vector3i){player_chunk.x + x,
                                                   player_chunk.y + y,
                                                   player_chunk.z + z});
            }
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

    // Check if new block would collide with player
    cuboid player_cuboid;
    cuboid_init(&player_cuboid, player->position.x - (COLLISION_BOX_X / 2),
                player->position.y - (COLLISION_BOX_Y / 2),
                player->position.z - (COLLISION_BOX_Z / 2), COLLISION_BOX_X,
                COLLISION_BOX_Y, COLLISION_BOX_Z);

    cuboid new_block_cubiod;
    cuboid_init(&new_block_cubiod, block_position.x, block_position.y,
                block_position.z, 1, 1, 1);

    // use glm aabb
    if (!collision_aabb_3d(player_cuboid, new_block_cubiod)) {
        world_set_block(world, type, block_position);
    }
}

void player_replace_block(player *player, world *world, block_type type) {
    player_set_target_block(player, world, type);
}
