#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void cleanup_handler(void *arg) { fprintf(stderr, "thread is exiting with cleanup: iteration count = %d\n", *(int*) arg); }

void* thread_func (void* arg) {
    int iteration = 1;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    pthread_cleanup_push(cleanup_handler, &iteration);

    while (1) {
        printf("thread is executing | iteration: %d\n", iteration++);
        sleep(1);
    }

    pthread_cleanup_pop(0);
    return NULL;
}

int main (int argc, char* argv[]) {
    if (argc != 2)
        return 1;

    int delay = atoi(argv[1]);
    if (delay <= 0)
        return 1;

    pthread_t thread;
    void* status;

    if (pthread_create(&thread, NULL, thread_func, NULL) != 0)
        return 1;

    sleep(delay);

    if (pthread_cancel(thread) != 0)
        return 1;

    if (pthread_join(thread, &status) != 0)
        return 1;

    if (status == PTHREAD_CANCELED)
        printf("thread canceled\n");
    else
        printf("thread exited normally\n");

    return 0;
}
