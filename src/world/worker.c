#include "worker.h"

#include "world.h"
#include "../math/math_util.h"

void job_init(job *job, chunk *chunk, job_type type, void *data) {
    job->chunk = chunk;
    job->type = type;
    job->data = data;
}

void *worker_thread_main(void *world_argument) {
    world *world = world_argument;

    while (true) {
        // printf("jobs: %d\n", safe_queue_length(&world->worker.jobs));
        // worker pointer
        pthread_mutex_lock(&world->worker.jobs_mutex);
        if (!safe_queue_is_empty(&world->worker.jobs)) {
            job *job = safe_queue_dequeue(&world->worker.jobs);

            safe_linked_list_insert_end(&world->chunks, job->chunk);

            pthread_mutex_unlock(&world->worker.jobs_mutex);

            switch (job->type) {
            case JOB_TYPE_GENERATE_TERRAIN:
                world_generate_chunk_terrain(world, job->chunk, 0);
                job->chunk->status = CHUNK_STATUS_DONE;

                printf("generating chunk terrain at pos ");
                vector3i_print(job->chunk->position);
            }
        } else {
            pthread_mutex_unlock(&world->worker.jobs_mutex);
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
            }
        }
    }

    return false;
}
