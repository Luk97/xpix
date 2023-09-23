#ifndef PROFILER_C_
#define PROFILER_C_


void init_profilers(int capacity);
void begin_profile(const char *name);
void end_profile();
void clear_profilers();

#endif // PROFILER_C_





#ifdef PROFILER_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
    const char *name;
    clock_t start;
} Profiler;

typedef struct {
    Profiler *profilers;
    int count;
    int capacity;
} ProfilerStack;

static ProfilerStack *stack;

void init_profilers(int capacity) {

    if (capacity <= 0) {
        fprintf(stderr, "[PROFILER ERROR] capacity \"%d\" must be a positive integer.\n", capacity);
        return;
    }

    stack = malloc(sizeof(ProfilerStack));
    if (!stack) {
        fprintf(stderr, "[PROFILER ERROR] malloc failed.\n");
        return;
    }

    stack->count = 0;
    stack->capacity = capacity;
    stack->profilers = malloc(sizeof(Profiler) * stack->capacity);
    if (!stack->profilers) {
        fprintf(stderr, "[PROFILER ERROR] malloc failed.\n");
        return;
    }
}

void begin_profile(const char *name) {
    if (stack->count >= stack->capacity) {
        fprintf(stderr, "[PROFILER ERROR] no capacity left for \"%s\"\n", name ? name : "null");
        return;
    }

    stack->count += 1;
    // TODO: append number to default name
    stack->profilers[stack->count].name = name ? name : "profiler";
    stack->profilers[stack->count].start = clock();
}

void end_profile() {
    if (stack->count <= 0) {
        fprintf(stderr, "[PROFILER ERROR] no profilers are currently running\n");
    }

    clock_t duration = clock() - stack->profilers[stack->count].start;

    printf(
        "[PROFILER INFO] %s took: %lds %ldms %ldns\n",
        stack->profilers[stack->count].name,
        duration / CLOCKS_PER_SEC,              // seconds
        duration % CLOCKS_PER_SEC / 1000,       // milliseconds
        duration % 1000                         // nanoseconds
    );

    stack->count -= 1;
}

void clear_profilers() {
    free(stack->profilers);
    free(stack);
}

#endif // PROFILER_IMPLEMENTATION
