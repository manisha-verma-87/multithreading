/**
 * reference : https://shivammitra.com/c/producer-consumer-problem-in-c/#
 **/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define BUFFER_SIZE 50
#define MAX_ITEMS 100
int items_produced = 0;
int items_consumed = 0;

int num[BUFFER_SIZE];

pthread_mutex_t m;
sem_t empty; // if there is any empty slot
sem_t full;  // if there is any item in the buffer

int in = 0;
int out = 0;

void* producer()
{
    int item;
    while(items_produced < MAX_ITEMS){
        item = rand();
        sem_wait(&empty);
        pthread_mutex_lock(&m);
        items_produced++;
        num[in] = item;
        printf("produced: %d\n", num[in]);
        in = (in+1)%BUFFER_SIZE;
        pthread_mutex_unlock(&m);
        sem_post(&full);
    }
}

void* consumer()
{
    while (items_consumed < MAX_ITEMS){
        sem_wait(&full);
        pthread_mutex_lock(&m);
        printf("consumed %d\n", num[out]);
        out = (out+1)%BUFFER_SIZE;
        items_consumed++;
        pthread_mutex_unlock(&m);
        sem_post(&empty);
    }
}

int main()
{
    pthread_t cons[5], prod[5];
    
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    
    for (int i = 0; i<5; i++){
        pthread_create(&prod[i], NULL, producer, NULL);
        pthread_create(&cons[i], NULL, consumer, NULL);
        
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }
    
    return 0;
}
