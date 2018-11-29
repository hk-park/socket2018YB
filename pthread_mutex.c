#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int ncount = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *do_sum(void *data){
	int i;
	int n = *((int *)data);
	for ( i=0;i<n;i++){
		printf("[%x]sum: %d\n", pthread_self(), ncount);
		pthread_mutex_lock(&mutex);
		ncount++;
		pthread_mutex_unlock(&mutex);
	}	
}
int main(){
	int thr_id;
	pthread_t p_thread[10];
	int status;
	int a = 5;
	int b = 10;
	thr_id = pthread_create(&p_thread[0], NULL, do_sum, (void *)&a);
	thr_id = pthread_create(&p_thread[1], NULL, do_sum, (void *)&b);
	pthread_join(p_thread[0], (void **)&status);
	pthread_join(p_thread[1], (void **)&status);
	printf("main program is ended\n");
	pthread_mutex_destroy(&mutex);
	return 0;
}


