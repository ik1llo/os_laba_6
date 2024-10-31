#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MIN_RANDOM 1
#define MAX_RANDOM 100

#define MAX_LINES 10

typedef struct {
    int thread_id;
    char message[50];
} ThreadData;

int generate_random (int min, int max) { return min + rand() % (max - min + 1); }

void* print_random_numbers (void* arg) {
    ThreadData* data = (ThreadData*) arg;
    int lines_count = generate_random(1, MAX_LINES);
    
    for (int k = 0; k < lines_count; k++) {
        int random_number = generate_random(MIN_RANDOM, MAX_RANDOM);

        printf("thread %d: %s %d\n", data->thread_id, data->message, random_number);
        sleep(1);
    }
    
    pthread_exit(NULL);
}

int main (int argc, char* argv[]) {
    if (argc != 2)
        return 1;

    int threads_count = atoi(argv[1]);
    if (threads_count <= 0)
        return 1;

    pthread_t threads[threads_count];
    ThreadData thread_data[threads_count];

    srand(time(NULL));

    for (int k = 0; k < threads_count; k++) {
        thread_data[k].thread_id = k;
        snprintf(thread_data[k].message, sizeof(thread_data[k].message), "random number generated: ");

        if (pthread_create(&threads[k], NULL, print_random_numbers, (void *)&thread_data[k]) != 0)
            return 1;
    }

    for (int k = 0; k < threads_count; k++)
        if (pthread_join(threads[k], NULL) != 0)
            return 1;

    printf("all threads have completed their tasks\n");
    return 0;
}
