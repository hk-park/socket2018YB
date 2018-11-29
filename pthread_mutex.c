#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *do_sum(void *data);
int ncount=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
	int thr_id, status, a=5, b=10;
	pthread_t p_thread[2];

	thr_id = pthread_create(&p_thread[0], NULL, do_sum, (void *)&a);
	thr_id = pthread_create(&p_thread[1], NULL, do_sum, (void *)&b);
	pthread_join(p_thread[0], (void **)&status);
	pthread_join(p_thread[1], (void **)&status);
	printf("main program is ended\n");
	pthread_mutex_destroy(&mutex);
	return 0;
}

void *do_sum(void *data){
	int i, n=*((int *)data);
	
	for(i=0;i<n;i++){
		printf("[%x]sum : %d\n", pthread_self(), ncount); //pthread_self() 자신의 pthread_id값 받아옴
		pthread_mutex_lock(&mutex);
		ncount++;
		pthread_mutex_unlock(&mutex);
	}
}
