#include "worker.h"

#include "world.h"
#include "chunk.h"
#include "../math/math_util.h"

void job_init(job *job, chunk *chunk, job_type type, void *data) {
    job->chunk = chunk;
    job->type = type;
    job->data = data;
}

void *worker_thread_main(void *world_argument) {
    world *world = world_argument;
    worker *worker = &world->worker;

    while (true) {
        // worker pointer
        pthread_mutex_lock(&worker->jobs_mutex);
        if (!safe_queue_is_empty(&worker->jobs)) {
            job *current_job = safe_queue_dequeue(&worker->jobs);

            safe_linked_list_insert_end(&world->chunks, current_job->chunk);

            pthread_mutex_unlock(&worker->jobs_mutex);

            switch (current_job->type) {
            case JOB_TYPE_GENERATE_TERRAIN:
                world_generate_chunk_terrain(
                    world, current_job->chunk,
                    ((job_data_generate_terrain *)current_job->data)->stage);

                int stage =
                    ((job_data_generate_terrain *)current_job->data)->stage;

                if (stage < 2) {

                    job_data_generate_terrain *job_data =
                        malloc(sizeof(job_data_generate_terrain));
                    job_data->stage = 2;

                    pthread_mutex_lock(&worker->jobs_mutex);
                    bool generation_job_exists =
                        worker_job_exists(worker, current_job->chunk->position,
                                          JOB_TYPE_GENERATE_TERRAIN, job_data);
                    pthread_mutex_unlock(&worker->jobs_mutex);
                } else {
                    job *mesh_job = malloc(sizeof(job));
                    job_init(mesh_job, current_job->chunk,
                             JOB_TYPE_GENERATE_MESH, NULL);
                }

                // worker_enqueue_job(&worker, mesh_job);

                printf("generating chunk terrain at pos ");
                vector3i_print(current_job->chunk->position);

            case JOB_TYPE_GENERATE_MESH:
                chunk_update_mesh(current_job->chunk);

                pthread_mutex_lock(&world->chunks_to_update_buffers_mutex);
                safe_queue_enqueue(&world->chunks_to_update_buffers,
                                   current_job->chunk);
                pthread_mutex_unlock(&world->chunks_to_update_buffers_mutex);
            }
        } else {
            pthread_mutex_unlock(&worker->jobs_mutex);
        }
    }

    pthread_exit(NULL);
}

void worker_init(worker *worker) {
    safe_queue_init(&worker->jobs);
    pthread_mutex_init(&worker->jobs_mutex, NULL);
}

void worker_enqueue_job(worker *worker, job *job) {
    safe_queue_enqueue(&worker->jobs, job);
}

job *worker_dequeue_job(worker *worker) {
    return safe_queue_dequeue(&worker->jobs);
}

job *worker_get_job(worker *worker, int index) {
    return safe_queue_get(&worker->jobs, index);
}

bool worker_job_exists(worker *worker, vector3i chunk_position, job_type type,
                       void *data) {
    for (int i = 0; i < safe_queue_length(&worker->jobs); i++) {
        job *current_job = safe_queue_get(&worker->jobs, i);

        if (current_job->type == type &&
            vector3i_equal(current_job->chunk->position, chunk_position)) {
            bool data_equal = false;

            switch (type) {
            case JOB_TYPE_GENERATE_TERRAIN:
                // ugly
                if (((job_data_generate_terrain *)current_job->data)->stage ==
                    ((job_data_generate_terrain *)data)->stage) {
                    return true;
                }

            default:
                return true;
            }
        }
    }

    return false;
}
