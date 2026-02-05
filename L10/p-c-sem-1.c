#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 2

sem_t empty;
sem_t full;

int buffer[10];

int count = 0;

void* producer(void* args) {
	while(1)
	{
        	int x = rand() % 100;
		sem_wait(&empty);		
		buffer[count] = x;
		count ++;
		sem_post(&full);

	}
}

void* consumer(void* args) {
	while(1)
	{
	        int y = -1 ;  //
		sem_wait(&full);
		y = buffer[count - 1];
        	count--;
		sem_post(&empty);
        	printf("Processing: %d\n", y);
	}
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_t th[THREAD_NUM];

    sem_init(&full,0,0);	//currently nothing there
    sem_init(&empty,0,10);   	//10 slots empty
    
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (i % 2 == 0) {
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    return 0;
}
