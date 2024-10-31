#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* thread_func (void* arg) {
    int delay = *((int*) arg);
    int iterations = delay * 2;

    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    for (int k = 1; k <= iterations; k++) {
        printf("thread is executing | iteration: %d\n", k);
        sleep(1);
    }

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

    if (pthread_create(&thread, NULL, thread_func, &delay) != 0)
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
