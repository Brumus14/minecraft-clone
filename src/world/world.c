#include "world.h"

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "../math/math_util.h"
#include "../util/stopwatch.h"
#include "block.h"
#include "worker.h"

int get_chunk_index(world *world, vector3i position) {
    for (int i = 0; i < linked_list_length(&world->chunks); i++) {
        chunk *current_chunk = (chunk *)linked_list_get(&world->chunks, i);

        if (vector3i_equal(current_chunk->position, position)) {
            return i;
        }
    }

    return -1;
}

unsigned int chunk_hasher(void *key) {
    vector3i position = *(vector3i *)key;
    return 0;
}

void world_init(world *world) {
    // linked_list_init(&world->chunks);
    hash_map_init(&world->chunks, CHUNKS_BUCKET_COUNT, sizeof(vector3i),
                  sizeof(), chunk_hasher);

    tilemap_init(&world->tilemap, "res/textures/atlas.png",
                 TEXTURE_FILTER_NEAREST, 16, 16, 1, 2);

    world->seed = random_range(0, 100);

    thread_pool_init(&world->workers, WORLD_WORKER_COUNT);
}

void world_load_chunk(world *world, vector3i position) {
    // Check not already loaded
    for (int i = 0; i < linked_list_length(&world->chunks); i++) {
        chunk *current_chunk = (chunk *)linked_list_get(&world->chunks, i);

        if (vector3i_equal(current_chunk->position, position)) {
            return;
        }
    }

    printf("added ");
    vector3i_print(position);

    chunk *new_chunk = malloc(sizeof(chunk));
    chunk_init(new_chunk, position, &world->tilemap);
    new_chunk->visible = false;

    linked_list_insert_end(&world->chunks, new_chunk);

    worker_generate_chunk_args *args =
        malloc(sizeof(worker_generate_chunk_args));
    args->chunk = new_chunk;
    args->seed = world->seed;

    thread_pool_schedule(&world->workers, worker_generate_chunk, args);
}

// remove chunks from chunks to generate if have moved out of render distance
// before generated
void world_unload_chunk(world *world, vector3i position) {
    int chunk_index = get_chunk_index(world, position);

    if (chunk_index == -1) {
        return;
    }

    chunk *chunk = linked_list_remove(&world->chunks, chunk_index);
    chunk_destroy(chunk);
    free(chunk);
}

void world_draw(world *world) {
    texture_bind(&world->tilemap.texture);

    for (int i = 0; i < linked_list_length(&world->chunks); i++) {
        chunk *chunk = linked_list_get(&world->chunks, i);

        if (chunk->state == CHUNK_STATE_NEEDS_BUFFERS) {
            chunk_update_buffers(chunk);
            chunk->state = CHUNK_STATE_READY;
        }

        if (chunk->state == CHUNK_STATE_READY) {
            chunk_draw(chunk);
        }
    }
}

// use mipmapping
block_type world_get_block(world *world, vector3d position) {
    // rename to chunks loaded
    vector3i chunk_position = {floor(position.x / CHUNK_SIZE_X),
                               floor(position.y / CHUNK_SIZE_Y),
                               floor(position.z / CHUNK_SIZE_Z)};

    int chunk_index = get_chunk_index(world, chunk_position);

    if (chunk_index == -1) {
        return BLOCK_TYPE_EMPTY;
    }

    chunk *chunk = linked_list_get(&world->chunks, chunk_index);

    vector3i block_chunk_position = {mod(floor(position.x), CHUNK_SIZE_X),
                                     mod(floor(position.y), CHUNK_SIZE_Y),
                                     mod(floor(position.z), CHUNK_SIZE_Z)};

    block_type block =
        chunk->blocks[block_chunk_position.z][block_chunk_position.y]
                     [block_chunk_position.x];

    return block;

    return BLOCK_TYPE_EMPTY;
}

void world_set_block(world *world, block_type type, vector3d position) {
    vector3i chunk_position = {floor(position.x / CHUNK_SIZE_X),
                               floor(position.y / CHUNK_SIZE_Y),
                               floor(position.z / CHUNK_SIZE_Z)};

    int chunk_index = get_chunk_index(world, chunk_position);

    if (chunk_index == -1) {
        return;
    }

    chunk *chunk = linked_list_get(&world->chunks, chunk_index);

    vector3i block_chunk_position = {mod(floor(position.x), CHUNK_SIZE_X),
                                     mod(floor(position.y), CHUNK_SIZE_Y),
                                     mod(floor(position.z), CHUNK_SIZE_Z)};

    chunk->blocks[block_chunk_position.z][block_chunk_position.y]
                 [block_chunk_position.x] = type;

    chunk_update_mesh(chunk);
    chunk->state = CHUNK_STATE_NEEDS_BUFFERS;
}
