#include"sched.h"
#include"pthread.h"
#include"stdio.h"
#include"semaphore.h"

int producer(void* args);
int consumer(void* args);
pthread_mutex_t mutex;
sem_t product;
sem_t warehouse;

char buffer[8][4];
int bp = 0;
pthread_t th[4];

main(int argc, char **argv)
{
	pthread_mutex_init(&mutex,NULL);
	sem_init(&product,0,0);
	sem_init(&warehouse,0,8);
	int arg;
	char* stack;
    int i ,j;
	for(i = 0,j = 0;i < 2;i ++)
	{
		arg = i;
		pthread_create(&th[j++],NULL,(void*)producer,(void*)&arg);

		pthread_create(&th[j++],NULL,(void*)consumer,(void*)&arg);
	}
	exit(1);
}


int producer(void* args)
{
	int id = *((int*)args);
	int i;
	for(i = 0 ;i < 10;i ++)
	{
		sleep(i+1);
		sem_wait(&warehouse);
		pthread_mutex_lock(&mutex);
		if(id == 0)
			strcpy(buffer[bp],"aaa\0");
		else
			strcpy(buffer[bp],"bbb\0");
		bp++;
		printf("producer %d produce %s in %d \n",id,buffer[bp],bp-1);
		pthread_mutex_unlock(&mutex);
		sem_post(&product);
	}
	printf("producer %d is over!\n",id);
}




int consumer(void* args)
{
	int id = *((int*)args);
	int i;
	for(i = 0;i < 10;i ++)
	{
		sleep(10-i);
		sem_wait(&product);
		pthread_mutex_lock(&mutex);
		bp--;
		printf("consumer %d get %s in %d \n",id,buffer[bp],bp);
		strcpy(buffer[bp],"zzz\0");
		pthread_mutex_unlock(&mutex);
		sem_post(&warehouse);
	}
	printf("consumer %d is over!\n",id);
}







































