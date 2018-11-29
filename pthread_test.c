#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *func_pthread(void *);		// 스레드가 수행할 함수
					// void * : java의 Object 개념. 모든 타입으로 형변환 가능
int main() {
	int status;
	pthread_t thread_id;

	status = pthread_create(&thread_id, NULL, func_pthread, NULL);	// 2번째 인자값 : 속성 정해주기
									// 3번째 함수의 이름
	if(status != 0) {
		printf("[ERROR] thread create FAIL!!.\n");
		exit(1);
	}

	printf("thread %x is created\n", thread_id);

	pthread_join(thread_id, (void **)&status);

	sleep(5);
	printf("main function END\n");

}

void *func_pthread(void *argv) {
	int i = 0;

	while(i < 10) {
		sleep(1);
		i++;
		printf("thread %dth executing....\n",i);
	}
}
