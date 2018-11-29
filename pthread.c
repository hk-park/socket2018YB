#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *func_pthread(void *); //스레드가 수행할 함수
//void * :  Java의 Object 개념, 모든 타입으로 형변환 가능

int main() {
	int status;
	pthread_t thread_id;

	pthread_create(&thread_id, NULL, func_pthread, NULL); //스레드를 생성할 함수
	if(status != 0) {
		printf("[ERROR] thread create fail\n");
		exit(1);
	}
	printf("thread %d is created\n", thread_id);
	pthread_join(&thread_id, (void **)&status);  //main함수가 종료되어도, 스레드 작업 마무리 할 수 있게 하는 함수
	//sleep(5);
	printf("main function end\n");
}

void *func_pthread(void *argv) {
	int i=0;
	while(i<10) {
		sleep(1);
		i++;
		printf("thread %dth executing...\n", i);
	}
}
