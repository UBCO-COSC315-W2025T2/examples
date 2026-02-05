#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 2

sem_t semEmpty;
sem_t semFull;

pthread_cond_t  empty, fill;
pthread_mutex_t mutexBuffer;

int buffer[10];

int count = 0;

void* producer(void* args) {
	while(1)
	{
        	int x = rand() % 100;
 		pthread_mutex_lock(&mutexBuffer);
        	
		while (count > 9)
    			pthread_cond_wait(&empty, &mutexBuffer);
		
		buffer[count] = x;
		count ++;

		pthread_cond_signal(&fill);
		pthread_mutex_unlock(&mutexBuffer);

	}
}

void* consumer(void* args) {
	while(1)
	{
	        int y = -1 ;  //
		pthread_mutex_lock(&mutexBuffer);
		while (count == 0)
			pthread_cond_wait(&fill, &mutexBuffer);

		y = buffer[count - 1];
        	count--;

		pthread_cond_signal(&empty);
	 	pthread_mutex_unlock(&mutexBuffer);
        	printf("Processing: %d\n", y);
	}
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexBuffer, NULL);	
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&fill, NULL);

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
