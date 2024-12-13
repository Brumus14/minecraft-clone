#include "world.h"

#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "FastNoiseLite.h"
#include "../math/math_util.h"
#include "../util/stopwatch.h"
#include "worker.h"

int get_chunk_index(world *world, vector3i position) {
    pthread_mutex_lock(&world->chunks_mutex);
    for (int i = 0; i < safe_linked_list_length(&world->chunks); i++) {
        chunk *current_chunk = (chunk *)safe_linked_list_get(&world->chunks, i);

        if (vector3i_equal(current_chunk->position, position)) {
            pthread_mutex_unlock(&world->chunks_mutex);
            return i;
        }
    }
    pthread_mutex_unlock(&world->chunks_mutex);

    return -1;
}

void world_generate_chunk_terrain(world *world, chunk *chunk, int stage) {
    if (stage == 1) {
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
    if (stage == 2) {
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

                    if (fnlGetNoise3D(&cave_noise, position.x * 2,
                                      position.y * 2, position.z * 2) > 0.3) {
                        chunk->blocks[z][y][x] = BLOCK_TYPE_EMPTY;
                    } else {
                        if (chunk->blocks[z][y][x] == BLOCK_TYPE_STONE) {
                            if (fnlGetNoise3D(&cave_noise, position.x * 2,
                                              position.y * 2,
                                              position.z * 2) > 0.3) {
                                chunk->blocks[z][y][x] = BLOCK_TYPE_EMPTY;
                            } else if (fnlGetNoise3D(&coal_noise,
                                                     position.x * 20,
                                                     position.y * 20,
                                                     position.z * 20) > 0.6) {
                                chunk->blocks[z][y][x] = BLOCK_TYPE_COAL;
                            } else if (fnlGetNoise3D(&diamond_noise,
                                                     position.x * 20,
                                                     position.y * 20,
                                                     position.z * 20) > 0.91) {
                                chunk->blocks[z][y][x] = BLOCK_TYPE_DIAMOND;
                            }
                        } else if (chunk->blocks[z][y][x] == BLOCK_TYPE_DIRT) {
                            if (fnlGetNoise3D(&cave_noise, position.x * 2,
                                              (position.y + 1) * 2,
                                              position.z * 2) > 0.3) {
                                chunk->blocks[z][y][x] = BLOCK_TYPE_GRASS;
                            }
                        }
                    }
                }
            }
        }
    }

    chunk->generation_stage = stage;
}

double total = 0;
int chunks_generated = 0;

// void *worker_thread_main(void *world_arg) {
//     world *world_pointer = (world *)world_arg;
//
//     // copy the data so dont have to keep chunks to generate locked for main
//     // thread
//     while (true) {
//         pthread_mutex_lock(&worker_jobs_mutex);
//         if (safe_queue_length(&world_pointer->worker_jobs) > 0) {
//             chunk *chunk = safe_queue_dequeue(&world_pointer->worker_jobs);
//
//             safe_linked_list_insert_end(&world_pointer->chunks, chunk);
//             pthread_mutex_unlock(&chunks_to_generate_mutex);
//
//             chunk->status = CHUNK_STATUS_DONE;
//
//             // stopwatch timer;
//             // stopwatch_start(&timer);
//
//             generate_chunk_terrain(world_pointer, chunk, 0);
//             generate_chunk_terrain(world_pointer, chunk, 1);
//
//             // stopwatch_end(&timer);
//             // total += stopwatch_elapsed(&timer);
//             // chunks_generated++;
//             // printf("%f\n", total / chunks_generated);
//
//             safe_queue_enqueue(&world_pointer->chunks_to_initial_update,
//             chunk);
//         } else {
//             pthread_mutex_unlock(&chunks_to_generate_mutex);
//         }
//     }
//
//     pthread_exit(NULL);
// }

void world_init(world *world) {
    tilemap_init(&world->tilemap, "res/textures/atlas.png",
                 TEXTURE_FILTER_NEAREST, 16, 16, 1, 2);

    safe_linked_list_init(&world->chunks);
    safe_queue_init(&world->chunks_to_update_buffers);

    world->seed = random_range(0, 100);

    worker_init(&world->worker);

    pthread_mutex_init(&world->chunks_mutex, NULL);
    pthread_mutex_init(&world->chunks_to_update_buffers_mutex, NULL);

    pthread_create(&world->worker_thread, NULL, worker_thread_main, world);
    pthread_detach(world->worker_thread);
}

void world_load_chunk(world *world, vector3i position) {
    pthread_mutex_lock(&world->chunks_mutex);
    for (int i = 0; i < safe_linked_list_length(&world->chunks); i++) {
        chunk *current_chunk = (chunk *)safe_linked_list_get(&world->chunks, i);

        if (vector3i_equal(current_chunk->position, position)) {
            pthread_mutex_unlock(&world->chunks_mutex);
            return;
        }
    }
    pthread_mutex_unlock(&world->chunks_mutex);

    job_data_generate_terrain *job_data =
        malloc(sizeof(job_data_generate_terrain));
    job_data->stage = 1;

    pthread_mutex_lock(&world->worker.jobs_mutex);
    bool generation_job_exists = worker_job_exists(
        &world->worker, position, JOB_TYPE_GENERATE_TERRAIN, job_data);
    pthread_mutex_unlock(&world->worker.jobs_mutex);

    if (generation_job_exists) {
        return;
    }

    printf("added ");
    vector3i_print(position);

    // memory leak
    chunk *new_chunk = malloc(sizeof(chunk));
    chunk_init(new_chunk, position, &world->tilemap);
    new_chunk->visible = false;

    job *chunk_generation_job = malloc(sizeof(job));

    job_init(chunk_generation_job, new_chunk, JOB_TYPE_GENERATE_TERRAIN,
             job_data);

    worker_enqueue_job(&world->worker, chunk_generation_job);
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

    pthread_mutex_lock(&world->chunks_to_update_buffers_mutex);
    for (int i = 0; i < safe_queue_length(&world->chunks_to_update_buffers);
         i++) {
        chunk *chunk = safe_queue_dequeue(&world->chunks_to_update_buffers);

        chunk_update_buffers(chunk);

        if (!chunk->visible) {
            chunk->visible = true;
        }
        printf("updating buffers for ");
        vector3i_print(chunk->position);
    }
    pthread_mutex_unlock(&world->chunks_to_update_buffers_mutex);

    pthread_mutex_lock(&world->chunks_mutex);
    for (int i = 0; i < safe_linked_list_length(&world->chunks); i++) {
        chunk *chunk = safe_linked_list_get(&world->chunks, i);

        // chunk_update_buffers(chunk);
        chunk_draw(chunk);
    }
    pthread_mutex_unlock(&world->chunks_mutex);
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

    chunk_update_mesh(chunk);
    chunk_update_buffers(chunk);
}
