#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
void *func_pthread(void *);//스레드가 수행할 함수
//void * - 자바로 치면 오브젝트개념, 모든 타입으로 형변환 가능

int main(){
	int status;
	pthread_t thread_id;
	
	status = pthread_create(&thread_id, NULL,func_pthread, NULL );//스레드 생성 함수(아이디,속성을 인자로(보통은 null).수행할함수이름,인자값(void *))
	if(status != 0){
		printf("[ERROR] thread create fail\n");
		exit(1);//보통 에러나면 1을 줌
	}
	printf("tread %c is created\n",thread_id);
	pthread_join(thread_id, (void **)&status);//아이디, 상태값

	sleep(5);

	printf("main function end\n");
}

void *func_pthread (void *argv){
	int i=0;
	while(i<10){
		sleep(1);
		i++;
		printf("thread %dth execution...\n", i);
	}
}
