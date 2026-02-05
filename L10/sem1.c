#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS     4

sem_t s;

void* worker(void *arg) {
    sem_wait(&s);                               //decrement
    sleep(1);
    printf("Hello from thread %d\n", *(int *)arg);
    sem_post(&s);	                        //increment
    //printf("waiting %d \r\n", num_wait);
    free(arg);
}

int main(int argc, char *argv[]) {
    pthread_t th[NUM_THREADS];

    sem_init(&s, 0, 2);

    int i;
    for (i = 0; i < NUM_THREADS; i++) {
	//watch out ->  why malloc needed here? (
	int* val = malloc(sizeof(int));
        *val = i;
        if (pthread_create(&th[i], NULL, &worker, val) != 0) {
            perror("Failed to create thread");
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    return 0;
}
