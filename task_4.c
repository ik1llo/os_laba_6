#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ITERATIONS_COUNT 100000

double pi = 0.0;
void* calculate_pi(void* arg) {
    int k;
    double term;
    
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    for (k = 0; k < ITERATIONS_COUNT; k++) {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

        term = (k % 2 == 0 ? 1.0 : -1.0) / (2 * k + 1);
        pi += term * 4;

        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        pthread_testcancel();
        printf("current π value after %d iterations: %.15f\n", k + 1, pi);
    }
    
    pthread_exit((void*) &pi);
}

int main (int argc, char* argv[]) {
    if (argc != 2)
        return 1;

    int delay = atoi(argv[1]);
    if (delay <= 0)
        return 1;

    pthread_t thread;
    void* status;

    if (pthread_create(&thread, NULL, calculate_pi, NULL) != 0)
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
