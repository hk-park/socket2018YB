//pthread_mutex.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int ncount;
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

void *do_sum(void *data)
{
	int	i;
	int	n = *((int *)data);
	for(i = 0; i < n; i++)
	{
		pthread_mutex_lock(&mutex);
        printf("[%x]", pthread_self()); //pthread_self() : 자신의 thread id 리턴
		printf("sum : %d\n", ncount);
		ncount ++;
		pthread_mutex_unlock(&mutex);
	}
}
int main( )
{
	int	thr_id;
	pthread_t p_thread[2];
	int	status;
	int	a ;
	ncount = 1;
	a = 3;
	thr_id = pthread_create(&p_thread[0], NULL, do_sum, (void *) &a);
    usleep(1); //1 usec sleep
    a = 10;
	thr_id = pthread_create(&p_thread[1], NULL, do_sum, (void *) &a);
	pthread_join(p_thread[0], (void **) &status);
	pthread_join(p_thread[1], (void **) &status);
	status = pthread_mutex_destroy(&mutex);
	printf("programing is end");
	return 0;
}
