#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
int ncount = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *do_sum(void *data){
	int i;
	int n = *((int*)data);
	for(i=0;i<n;i++){
		pthread_mutex_lock(&mutex);
		printf("[%x]",pthread_self());
		printf("sum : %d\n",ncount);
		ncount++;
		pthread_mutex_unlock(&mutex);
	}

}

int main()
{
	int thr_id;
	int a=3;
	int status;
	pthread_t thread_id[2];
	thr_id = pthread_create(&thread_id[0],NULL,do_sum,(void *) &a);
	usleep(1);
	thr_id = pthread_create(&thread_id[1],NULL,do_sum,(void *) &a);
	pthread_join(thread_id[0],(void **) &status);
	pthread_join(thread_id[1],(void **) &status);
	status = pthread_mutex_destroy(&mutex);
	printf("main exit\n");
	return 0;
}
