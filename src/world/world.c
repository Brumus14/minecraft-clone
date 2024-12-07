#include "world.h"

#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "FastNoiseLite.h"
#include "../math/math_util.h"
#include "../util/stopwatch.h"

// pthread_mutex_t chunks_to_generate_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t chunk_to_generate_condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t chunks_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t chunks_to_generate_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t chunks_to_initial_update_mutex = PTHREAD_MUTEX_INITIALIZER;

int get_chunk_index(world *world, vector3i position) {
    pthread_mutex_lock(&chunks_mutex);
    for (int i = 0; i < safe_linked_list_length(&world->chunks); i++) {
        chunk *current_chunk = (chunk *)safe_linked_list_get(&world->chunks, i);

        if (vector3i_equal(current_chunk->position, position)) {
            pthread_mutex_unlock(&chunks_mutex);
            return i;
        }
    }
    pthread_mutex_unlock(&chunks_mutex);

    return -1;
}

void generate_chunk_terrain(world *world, chunk *chunk, int stage) {
    if (stage == 0) {
        fnl_state height_noise = fnlCreateState();
        height_noise.noise_type = FNL_NOISE_PERLIN;
        height_noise.seed = world->seed;

        vector3i chunk_block_position;
        vector3i_init(&chunk_block_position, chunk->position.x * CHUNK_SIZE_X,
                      chunk->position.y * CHUNK_SIZE_Y,
                      chunk->position.z * CHUNK_SIZE_Z);

        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            for (int y = 0; y < CHUNK_SIZE_Y; y++) {
                for (int x = 0; x < CHUNK_SIZE_X; x++) {
                    vector3i position;
                    vector3i_init(&position, chunk_block_position.x + x,
                                  chunk_block_position.y + y,
                                  chunk_block_position.z + z);

                    block_type type = BLOCK_TYPE_EMPTY;

                    // float noise_value = noise3(position.x * 0.03,
                    //                            position.z * 0.03,
                    //                            world->seed) *
                    //                     16;

                    float height_value =
                        fnlGetNoise2D(&height_noise, position.x * 2,
                                      position.z * 2) *
                        16;

                    if (position.y < height_value - 8) {
                        type = BLOCK_TYPE_STONE;
                    } else if (position.y < height_value) {
                        type = BLOCK_TYPE_DIRT;
                    } else if (position.y == ceil(height_value)) {
                        type = BLOCK_TYPE_GRASS;
                    }

                    chunk->blocks[z][y][x] = type;
                }
            }
        }
    }

    // better ore generation
    // use octaves and make noise generator abstraction
    if (stage == 1) {
        fnl_state cave_noise = fnlCreateState();
        cave_noise.noise_type = FNL_NOISE_OPENSIMPLEX2S;
        cave_noise.seed = world->seed;

        fnl_state coal_noise = fnlCreateState();
        coal_noise.noise_type = FNL_NOISE_PERLIN;
        coal_noise.seed = world->seed;

        fnl_state diamond_noise = fnlCreateState();
        diamond_noise.noise_type = FNL_NOISE_OPENSIMPLEX2S;
        diamond_noise.seed = world->seed + 1;

        vector3i chunk_block_position;
        vector3i_init(&chunk_block_position, chunk->position.x * CHUNK_SIZE_X,
                      chunk->position.y * CHUNK_SIZE_Y,
                      chunk->position.z * CHUNK_SIZE_Z);

        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            for (int y = 0; y < CHUNK_SIZE_Y; y++) {
                for (int x = 0; x < CHUNK_SIZE_X; x++) {
                    vector3i position = {chunk_block_position.x + x,
                                         chunk_block_position.y + y,
                                         chunk_block_position.z + z};

                    if (chunk->blocks[z][y][x] == BLOCK_TYPE_STONE) {
                        if (fnlGetNoise3D(&cave_noise, position.x * 2,
                                          position.y * 2,
                                          position.z * 2) > 0.3) {
                            chunk->blocks[z][y][x] = BLOCK_TYPE_EMPTY;
                        } else if (fnlGetNoise3D(&coal_noise, position.x * 20,
                                                 position.y * 20,
                                                 position.z * 20) > 0.6) {
                            chunk->blocks[z][y][x] = BLOCK_TYPE_COAL;
                        } else if (fnlGetNoise3D(&diamond_noise,
                                                 position.x * 20,
                                                 position.y * 20,
                                                 position.z * 20) > 0.91) {
                            chunk->blocks[z][y][x] = BLOCK_TYPE_DIAMOND;
                        }
                    }
                }
            }
        }
    }
}

double total = 0;
int chunks_generated = 0;

void *generation_thread_main(void *world_arg) {
    world *world_pointer = (world *)world_arg;

    // copy the data so dont have to keep chunks to generate locked for main
    // thread
    while (true) {
        pthread_mutex_lock(&chunks_to_generate_mutex);
        if (safe_queue_length(&world_pointer->chunks_to_generate) > 0) {
            chunk *chunk =
                safe_queue_dequeue(&world_pointer->chunks_to_generate);

            safe_linked_list_insert_end(&world_pointer->chunks, chunk);
            pthread_mutex_unlock(&chunks_to_generate_mutex);

            chunk->status = CHUNK_STATUS_DONE;

            stopwatch timer;
            stopwatch_start(&timer);

            generate_chunk_terrain(world_pointer, chunk, 0);
            generate_chunk_terrain(world_pointer, chunk, 1);

            stopwatch_end(&timer);
            total += stopwatch_elapsed(&timer);
            chunks_generated++;
            printf("%f\n", total / chunks_generated);

            safe_queue_enqueue(&world_pointer->chunks_to_initial_update, chunk);
        } else {
            pthread_mutex_unlock(&chunks_to_generate_mutex);
        }
    }

    pthread_exit(NULL);
}

void world_init(world *world) {
    tilemap_init(&world->tilemap, "res/textures/atlas.png",
                 TEXTURE_FILTER_NEAREST, 16, 16, 1, 2);

    safe_linked_list_init(&world->chunks);
    safe_queue_init(&world->chunks_to_generate);
    safe_queue_init(&world->chunks_to_initial_update);

    world->seed = random_range(0, 100);

    pthread_create(&world->generation_thread, NULL, generation_thread_main,
                   world);
    pthread_detach(world->generation_thread);
}

void world_load_chunk(world *world, vector3i position) {
    pthread_mutex_lock(&chunks_mutex);
    for (int i = 0; i < safe_linked_list_length(&world->chunks); i++) {
        chunk *current_chunk = (chunk *)safe_linked_list_get(&world->chunks, i);

        if (vector3i_equal(current_chunk->position, position)) {
            pthread_mutex_unlock(&chunks_mutex);
            return;
        }
    }
    pthread_mutex_unlock(&chunks_mutex);

    pthread_mutex_lock(&chunks_to_generate_mutex);
    for (int i = 0; i < safe_queue_length(&world->chunks_to_generate); i++) {
        chunk *current_chunk =
            (chunk *)safe_queue_get(&world->chunks_to_generate, i);

        if (vector3i_equal(current_chunk->position, position)) {
            pthread_mutex_unlock(&chunks_to_generate_mutex);
            return;
        }
    }
    pthread_mutex_unlock(&chunks_to_generate_mutex);

    printf("added ");
    vector3i_print(position);

    chunk *new_chunk = malloc(sizeof(chunk));
    chunk_init(new_chunk, position, &world->tilemap);

    safe_queue_enqueue(&world->chunks_to_generate, new_chunk);

    printf("%d\n", safe_queue_length(&world->chunks_to_generate));

    // vector3i_print(new_chunk->position);
}

// remove chunks from chunks to generate if have moved out of render distance
// before generated
void world_unload_chunk(world *world, vector3i position) {
    int chunk_index = get_chunk_index(world, position);

    if (chunk_index == -1) {
        return;
    }

    safe_linked_list_remove(&world->chunks, chunk_index);
}

void world_draw(world *world) {
    texture_bind(&world->tilemap.texture);

    pthread_mutex_lock(&chunks_to_initial_update_mutex);
    while (!safe_queue_is_empty(&world->chunks_to_initial_update)) {
        chunk_update(safe_queue_dequeue(&world->chunks_to_initial_update));
    }
    pthread_mutex_unlock(&chunks_to_initial_update_mutex);

    pthread_mutex_lock(&chunks_mutex);
    for (int i = 0; i < safe_linked_list_length(&world->chunks); i++) {
        chunk_draw(safe_linked_list_get(&world->chunks, i));
    }
    pthread_mutex_unlock(&chunks_mutex);
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

    chunk *chunk = safe_linked_list_get(&world->chunks, chunk_index);

    vector3i block_chunk_position = {mod(floor(position.x), CHUNK_SIZE_X),
                                     mod(floor(position.y), CHUNK_SIZE_Y),
                                     mod(floor(position.z), CHUNK_SIZE_Z)};

    block_type block =
        chunk->blocks[block_chunk_position.z][block_chunk_position.y]
                     [block_chunk_position.x];

    return block;
}

void world_set_block(world *world, block_type type, vector3d position) {
    vector3i chunk_position = {floor(position.x / CHUNK_SIZE_X),
                               floor(position.y / CHUNK_SIZE_Y),
                               floor(position.z / CHUNK_SIZE_Z)};

    int chunk_index = get_chunk_index(world, chunk_position);

    if (chunk_index == -1) {
        return;
    }

    chunk *chunk = safe_linked_list_get(&world->chunks, chunk_index);

    vector3i block_chunk_position = {mod(floor(position.x), CHUNK_SIZE_X),
                                     mod(floor(position.y), CHUNK_SIZE_Y),
                                     mod(floor(position.z), CHUNK_SIZE_Z)};

    chunk->blocks[block_chunk_position.z][block_chunk_position.y]
                 [block_chunk_position.x] = type;

    chunk_update(chunk);
}
