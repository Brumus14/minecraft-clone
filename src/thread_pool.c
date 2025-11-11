#include "thread_pool.h"

#include "data_structures/queue.h"
#include "data_structures/safe_queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_pool_thread_main(void *arg) {
    thread_pool *pool = (thread_pool *)arg;
    queue *tasks = &pool->tasks;
    pthread_mutex_t *tasks_mutex = &pool->tasks_mutex;
    pthread_cond_t *task_available = &pool->task_available;

    // Add destroy flag
    while (true) {
        pthread_mutex_lock(tasks_mutex);

        while (queue_is_empty(tasks)) {
            pthread_cond_wait(task_available, tasks_mutex);
            printf("waiting\n");
        }

        thread_pool_task *task = queue_dequeue(tasks);
        pthread_mutex_unlock(tasks_mutex);

        if (task != NULL) {
            // Execute the task
            task->function(task->argument);
            free(task->argument);
            free(task);
        }
    }

    return NULL;
}

void thread_pool_init(thread_pool *pool, unsigned int count) {
    pool->thread_count = count;
    pool->threads = malloc(sizeof(pthread_t) * count);
    queue_init(&pool->tasks);
    pthread_mutex_init(&pool->tasks_mutex, NULL);
    pthread_cond_init(&pool->task_available, NULL);

    for (int i = 0; i < count; i++) {
        pthread_create(&pool->threads[i], NULL, thread_pool_thread_main, pool);
    }
}

void thread_pool_destroy(thread_pool *pool) {
    queue_destroy(&pool->tasks);
    pthread_mutex_destroy(&pool->tasks_mutex);
    pthread_cond_destroy(&pool->task_available);

    for (int i = 0; i < pool->thread_count; i++) {
        pthread_cancel(pool->threads[i]);
    }

    free(pool->threads);
}

void thread_pool_schedule(thread_pool *pool, thread_pool_task_function function,
                          void *argument) {
    pthread_mutex_lock(&pool->tasks_mutex);

    thread_pool_task *task = malloc(sizeof(thread_pool_task));
    task->function = function;
    task->argument = argument;

    queue_enqueue(&pool->tasks, task);
    pthread_mutex_unlock(&pool->tasks_mutex);

    pthread_cond_signal(&pool->task_available);
}
