#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *func_pthread(void *); //스레드가 수행할 함수
	//보이드 포인터는 자바으 ㅣobject 클래스와 같음. 모든 형을 담을 수 있음.

int main() {
	int status;
	pthread_t thread_id;

	status = pthread_create(&thread_id, NULL, func_pthread, NULL);
	//* * 수행할 함수, 그 인자값
	if(status != 0){
		printf("[ERROR] thread create fail");
		exit(1);	
	}
	printf("thread %x is created\n", thread_id);
			
	pthread_join(thread_id, (void **)&status);
	sleep(5);
	printf("main function end\n");


}
void *func_pthread(void *argv){
	int i = 0;
	while(i<10)
	{
		sleep(1);
		i++;
		printf("thresd %d th executeing...\n",i);
	}
}

