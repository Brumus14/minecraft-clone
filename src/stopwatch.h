#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <time.h>

typedef struct stopwatch {
    struct timespec start_time;
    struct timespec end_time;
} stopwatch;

void stopwatch_start(stopwatch *stopwatch);
void stopwatch_end(stopwatch *stopwatch);
double stopwatch_elapsed(stopwatch *stopwatch);
double stopwatch_time(stopwatch *stopwatch);

#endif
