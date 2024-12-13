#ifndef WORKER_H
#define WORKER_H

#include "chunk.h"
#include "../data_structures/safe_queue.h"

typedef enum job_type {
    JOB_TYPE_GENERATE_TERRAIN,
    JOB_TYPE_GENERATE_MESH,
} job_type;

typedef struct job_data_generate_terrain {
    int stage;
} job_data_generate_terrain;

typedef struct job {
    chunk *chunk;
    job_type type;
    void *data;
} job;

void job_init(job *job, chunk *chunk, job_type type, void *data);

typedef struct worker {
    safe_queue jobs;
    pthread_mutex_t jobs_mutex;
} worker;

void *worker_thread_main(void *world_argument);

void worker_init(worker *worker);
void worker_enqueue_job(worker *worker, job *job);
job *worker_dequeue_job(worker *worker);
job *worker_get_job(worker *worker, int index);
bool worker_job_exists(worker *worker, vector3i chunk_position, job_type type,
                       void *data);

#endif
