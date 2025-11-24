#include "world.h"

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "../math/math_util.h"
#include "../util/stopwatch.h"
#include "chunk.h"
#include "block.h"
#include "worker.h"

unsigned int chunk_hasher(void *key) {
    vector3i position = *(vector3i *)key;
    return position.x;
}

void world_init(world *world) {
    // linked_list_init(&world->chunks);
    hash_map_init(&world->chunks, CHUNKS_BUCKET_COUNT, sizeof(vector3i),
                  sizeof(chunk), chunk_hasher);

    tilemap_init(&world->tilemap, "res/textures/atlas.png",
                 TEXTURE_FILTER_NEAREST, 16, 16, 1, 2);

    world->seed = random_range(0, 100);

    thread_pool_init(&world->workers, WORLD_WORKER_COUNT);
}

void world_load_chunk(world *world, vector3i position) {
    // Check not already loaded
    // pthread_rwlock_rdlock(&world->chunks_lock);
    bool loaded = hash_map_get(&world->chunks, &position) != NULL;
    // pthread_rwlock_unlock(&world->chunks_lock);

    if (loaded) {
        return;
    }

    printf("added ");
    vector3i_print(position);

    chunk *new_chunk = malloc(sizeof(chunk));
    chunk_init(new_chunk, position, &world->tilemap);
    atomic_store(&new_chunk->visible, false);

    // pthread_rwlock_wrlock(&world->chunks_lock);
    chunk *old_value = hash_map_put(&world->chunks, &position, new_chunk);
    free(old_value);
    // pthread_rwlock_unlock(&world->chunks_lock);

    worker_generate_chunk_args *args =
        malloc(sizeof(worker_generate_chunk_args));
    args->chunk = new_chunk;
    args->seed = world->seed;

    thread_pool_schedule(&world->workers, worker_generate_chunk, args);
}

// remove chunks from chunks to generate if have moved out of render distance
// before generated
void world_unload_chunk(world *world, vector3i position) {
    // pthread_rwlock_wrlock(&world->chunks_lock);
    // chunk *chunk = hash_map_get(&world->chunks, &position);
    // pthread_mutex_lock(&chunk->lock);
    // pthread_rwlock_unlock(&world->chunks_lock);

    chunk *chunk = hash_map_remove(&world->chunks, &position);
    atomic_store(&chunk->visible, false);
    atomic_store(&chunk->unloaded, true);
}

void world_draw_chunk(void *key, void *value, void *context) {
    vector3i *chunk_position = key;
    // pthread_rwlock_rdlock(&world->chunks_lock);
    chunk *chunk = value;
    // pthread_rwlock_unlock(&world->chunks_lock);

    // if (chunk == NULL) {
    //     return;
    // }

    if (atomic_load(&chunk->state) == CHUNK_STATE_NEEDS_BUFFERS) {
        pthread_mutex_lock(&chunk->lock);
        chunk_update_buffers(chunk);
        pthread_mutex_unlock(&chunk->lock);

        atomic_store(&chunk->state, CHUNK_STATE_READY);
    }

    if (atomic_load(&chunk->state) == CHUNK_STATE_READY) {
        pthread_mutex_lock(&chunk->lock);
        chunk_draw(chunk);
        pthread_mutex_unlock(&chunk->lock);
    }
}

void world_draw(world *world) {
    texture_bind(&world->tilemap.texture);

    hash_map_for_each(&world->chunks, world_draw_chunk, NULL);
}

// use mipmapping
// TODO: Should this be vector3i?
block_type world_get_block(world *world, vector3d position) {
    // rename to chunks loaded
    vector3i chunk_position = {floor(position.x / CHUNK_SIZE_X),
                               floor(position.y / CHUNK_SIZE_Y),
                               floor(position.z / CHUNK_SIZE_Z)};

    // pthread_rwlock_rdlock(&world->chunks_lock);
    chunk *chunk = hash_map_get(&world->chunks, &chunk_position);
    // pthread_rwlock_unlock(&world->chunks_lock);

    if (chunk == NULL) {
        return BLOCK_TYPE_EMPTY;
    }

    pthread_mutex_lock(&chunk->lock);
    vector3i block_chunk_position = {mod(floor(position.x), CHUNK_SIZE_X),
                                     mod(floor(position.y), CHUNK_SIZE_Y),
                                     mod(floor(position.z), CHUNK_SIZE_Z)};

    block_type block =
        chunk_get_block(chunk, block_chunk_position.x, block_chunk_position.y,
                        block_chunk_position.z);
    pthread_mutex_unlock(&chunk->lock);

    return block;
}

void world_set_block(world *world, block_type type, vector3d position) {
    vector3i chunk_position = {floor(position.x / CHUNK_SIZE_X),
                               floor(position.y / CHUNK_SIZE_Y),
                               floor(position.z / CHUNK_SIZE_Z)};

    // pthread_rwlock_rdlock(&world->chunks_lock);
    chunk *chunk = hash_map_get(&world->chunks, &chunk_position);
    // pthread_rwlock_unlock(&world->chunks_lock);

    if (chunk == NULL) {
        return;
    }

    pthread_mutex_lock(&chunk->lock);
    vector3i block_chunk_position = {mod(floor(position.x), CHUNK_SIZE_X),
                                     mod(floor(position.y), CHUNK_SIZE_Y),
                                     mod(floor(position.z), CHUNK_SIZE_Z)};

    chunk_set_block(chunk, block_chunk_position.x, block_chunk_position.y,
                    block_chunk_position.z, type);
    chunk_update_mesh(chunk);
    pthread_mutex_unlock(&chunk->lock);

    atomic_store(&chunk->state, CHUNK_STATE_NEEDS_BUFFERS);
}
