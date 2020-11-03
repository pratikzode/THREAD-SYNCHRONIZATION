#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<semaphore.h>

#define BUFFERSIZE 5

int in=0, out=0;

sem_t full, empty;
pthread_mutex_t mutex;
int array[BUFFERSIZE];

void *producerMethod(void *data)
{
	int i, num, id;
	id=(int)data;
	for(i=0;i<3;i++)
	{
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
	
		num=id;
		array[in]=num;
		printf("\nProducer: %d\nProduced value: %d\nStored at Location: %d\n", id, array[in], in);	
		
		in=(in+1)%BUFFERSIZE;
		
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
		sleep(2);
	}
}

void *consumerMethod(void *data)
{
	int i, num, r;
	do
	{
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		
		num=array[out];
		printf("\n\tConsumer Consumed: %d\n\tFrom Location: %d\n", num, out);
		out=(out+1)%BUFFERSIZE;
		
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
		sleep(3);
		sem_getvalue(&empty, &r);
		
	}
	while(r!=BUFFERSIZE);		
}

int main()
{
	int r, i;
	pthread_t producer[4], consumer;
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, 5);
	
	for(i=0;i<3;i++)
		pthread_create(&producer[i], NULL, producerMethod ,(void *)i);  

	pthread_create(&consumer, NULL,consumerMethod , NULL);
	
	for(i=0; i<3;i++)
		pthread_join(producer[i], NULL);
	
	pthread_join(consumer, NULL);
	return 0;
}

