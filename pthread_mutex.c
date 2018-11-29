#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n =0 ;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //초기화 (전역변수니까 쓰레드끼리 공유되)
void *do_sum(void *data)
{
	int i;
	int max = *((int *)data);
	for ( i = 0; i < max; i ++)
	{
		printf("[%x] sum: %d \n",pthread_self(),n);
		pthread_mutex_lock(&mutex);  //뮤텍스 락, 언락은 거리?가 최대한 짧은게 좋다. 꼭 필요한 부분만 락을 걸어놓으라는 뜻
		n++;
		pthread_mutex_unlock(&mutex);
	}
}

int main(){
	int thr_id;
	pthread_t p_thread[2];
	int status;
	int a[2] = { 5, 10 };
	thr_id = pthread_create(&p_thread[0], NULL, do_sum, (void *)&a[0]);	
	thr_id = pthread_create(&p_thread[1], NULL, do_sum, (void *)&a[1]);
	pthread_join(p_thread[0], (void **)&status);
	pthread_join(p_thread[1], (void **)&status);
	
	printf("main program is ended\n");
	pthread_mutex_destroy(&mutex);
}	//,싱글 프로세스 가상머신에선 뮤텍스 안써도 되기는한다.
