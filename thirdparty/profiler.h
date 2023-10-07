#ifndef PROFILER_H_
#define PROFILER_H_

#ifndef PROFILER_START_CAPACITY
    #define PROFILER_START_CAPACITY 8
#endif // PROFILER_START_CAPACITY

#ifndef PROFILER_DEFAULT_NAME
    #define PROFILER_DEFAULT_NAME "Default Profiler"
#endif // PROFILER_DEFAULT_NAME

#define PROFILER_USEC 1
#define PROFILER_MS   PROFILER_USEC * 1000
#define PROFILER_SEC  PROFILER_MS   * 1000

typedef enum {
    PROFILER_LOG_INFO    = 0,
    PROFILER_LOG_WARNING = 1,
    PROFILER_LOG_ERROR   = 2
} ProfilerLoglevel;

void profiler_init(ProfilerLoglevel loglevel);
void profiler_begin(const char *name, long threshold);
void profiler_end();
void profiler_destroy();

#endif // PROFILER _H_



#ifdef PROFILER_IMPLEMENTATION

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    const char *name;
    unsigned int thresholdUSec;
    struct timeval buffer;
} Profiler;

typedef struct {
    Profiler *profilers;
    int count;
    int capacity;
    ProfilerLoglevel loglevel;
} ProfilerStack;

static ProfilerStack *ps; // profilerStack

void profiler_init(ProfilerLoglevel loglevel) {

    ps = malloc(sizeof(ProfilerStack));
    if (!ps) {
        fprintf(stderr, "[PROFILER ERROR] malloc failed\n");
        return;
    }

    ps->count = -1;
    ps->capacity = PROFILER_START_CAPACITY;
    ps->profilers = malloc(sizeof(Profiler) * ps->capacity);
    if (!ps->profilers) {
        fprintf(stderr, "[PROFILER ERROR] malloc failed\n");
        return;
    }

    ps->loglevel = (loglevel >= PROFILER_LOG_INFO && loglevel <= PROFILER_LOG_ERROR) ? loglevel : PROFILER_LOG_INFO; 

    // TODO: index 0 for over all profiler time???
}

void profiler_begin(const char *name, long threshold) {

    ps->count += 1;

    if (ps->count >= ps->capacity) {
        ps->capacity *= 2;
        ps->profilers = realloc(ps->profilers, sizeof(Profiler) * ps->capacity);
        if (!ps->profilers) {
            fprintf(stderr, "[PROFILER ERROR] realloc failed\n");
            return;
        }
    }

    ps->profilers[ps->count].name = name ? name : PROFILER_DEFAULT_NAME;
    ps->profilers[ps->count].thresholdUSec = threshold > 0 ? threshold : 0;
    
    gettimeofday(&ps->profilers[ps->count].buffer, NULL);
}

static int threshold_exceeded(struct timeval t) {
    return(
        (ps->profilers[ps->count].thresholdUSec > 0) &&
        (t.tv_sec * PROFILER_SEC + t.tv_usec >= ps->profilers[ps->count].thresholdUSec)
    );
}

void profiler_end() {

    if (ps->count < 0) {
        fprintf(stderr, "[PROFILER ERROR] profiler count became negative\n");
        return;
    }

    struct timeval end;
    gettimeofday(&end, NULL);

    end.tv_sec -= ps->profilers[ps->count].buffer.tv_sec;
    end.tv_usec -= ps->profilers[ps->count].buffer.tv_usec;

    if (end.tv_usec < 0) {
        end.tv_sec -= 1;
        end.tv_usec += 1000000;
    }

    if (ps->loglevel <= PROFILER_LOG_WARNING && threshold_exceeded(end)) {

        long overtime = (end.tv_sec * 1000000 + end.tv_usec) - ps->profilers[ps->count].thresholdUSec;

        printf(
            "[PROFILER WARNING] %s: %lds %ldms %ldusec | overtime: %lds %ldms %ldusec\n",
            ps->profilers[ps->count].name,
            end.tv_sec,                // seconds
            end.tv_usec / 1000,        // milli seconds
            end.tv_usec % 1000,        // micro seconds
            overtime / 1000000,        // overtime seconds
            overtime % 1000000 / 1000, // overtime milli seconds
            overtime % 1000  // overtime micro seconds
        );

    } else if (ps->loglevel <= PROFILER_LOG_INFO) {

        printf(
            "[PROFILER INFO] %s: %lds %ldms %ldusec\n",
            ps->profilers[ps->count].name,
            end.tv_sec,         // seconds
            end.tv_usec / 1000, // milli seconds
            end.tv_usec % 1000  // micro seconds
        );
    }

    ps->count -= 1;
}

void profiler_destroy() {
    free(ps->profilers);
    free(ps);
}

#endif // PROFILER_IMPLEMENTATION
