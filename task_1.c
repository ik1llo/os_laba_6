#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_THREADS_AMOUNT 100

int thread_count;
int array[MAX_THREADS_AMOUNT];
int array_filled_count = 0;

pthread_mutex_t mutex;

void* calculate_thread (void* arg) {
    int thread_number = *(int*) arg;

    sleep(thread_number);

    int random_value = rand() % 10 + 1;

    pthread_mutex_lock(&mutex);
    array[thread_number] = random_value;
    array_filled_count++;
    pthread_mutex_unlock(&mutex);

    sleep(random_value);
    pthread_exit(NULL);
}

void* monitor_thread (void* arg) {
    while (1) {
        sleep(1);

        pthread_mutex_lock(&mutex);

        printf("array status: ");
        for (int k = 0; k < thread_count; k++)
            printf("%d ", array[k]);
        printf("\n");

        if (array_filled_count >= thread_count) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main (int argc, char* argv[]) {
    if (argc != 2)
        return 1;

    thread_count = atoi(argv[1]);
    if (thread_count > MAX_THREADS_AMOUNT || thread_count <= 0)
        return 1;

    for (int k = 0; k < thread_count; k++)
        array[k] = 0;
    pthread_mutex_init(&mutex, NULL);
    srand(time(NULL));

    pthread_t monitor;
    pthread_create(&monitor, NULL, monitor_thread, NULL);
    pthread_detach(monitor);

    pthread_t threads[thread_count];
    int thread_values[thread_count];
    for (int k = 0; k < thread_count; k++) {
        thread_values[k] = k;
        pthread_create(&threads[k], NULL, calculate_thread, &thread_values[k]);
    }

    for (int k = 0; k < thread_count; k++)
        pthread_join(threads[k], NULL);

    pthread_mutex_destroy(&mutex);

    printf("all threads have finished its execution\n");
    return 0;
}