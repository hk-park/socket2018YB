#include <stdio.h>
#include <pthread.h> //gcc할때 추가적으로 -lpthread 작성.
#include <stdlib.h>

void *func_pthread(void *argv); //스레드가 수행할 함수.
//void * : Java의 Object 개념. 모든 타입으로 형변환 가능.

int main(){
	int status;
	pthread_t thread_id;
	
	status = pthread_create(&thread_id, NULL, func_pthread, NULL); //스레드 생성.
	//2번째 NULL : 스레드의 속성, 보통 NULL.
	//func_pthread로 받을 파라미터값. 이 코드에선 받지 않아 NULL.
	if(status != 0){
		printf("[ERROR] thread create fail\n");
		exit(1);
	}
	printf("thread %x is created\n", thread_id);
	pthread_join(thread_id, (void **)&status);
	//sleep(5);
	printf("main function end\n");
}

void *func_pthread(void *argv){
	int i=0;
	while(i<10){
		sleep(1);
		i++;
		printf("thread %dth executing...\n", i);
	}
}
