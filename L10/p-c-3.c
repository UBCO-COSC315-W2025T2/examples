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

pthread_mutex_t mutexBuffer;

int buffer[10];

int count = 0;

void* producer(void* args) {
	while(1)
	{
        	int x = rand() % 100;
 		pthread_mutex_lock(&mutexBuffer);
        	if (count < 10)	//issues? 
		{
			buffer[count] = x;
			count ++;
		}
		//
		pthread_mutex_unlock(&mutexBuffer);
		//sleep(1);

	}
}

void* consumer(void* args) {
	while(1)
	{
	        int y = -1 ;  //
		pthread_mutex_lock(&mutexBuffer);
        	if (count > 0)
		{
			y = buffer[count - 1];
        		count--;
		}
	 	pthread_mutex_unlock(&mutexBuffer);
        	printf("Processing: %d\n", y);
	}
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexBuffer, NULL);	
    
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
