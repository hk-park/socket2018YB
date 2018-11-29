#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *func_phtread(void *);//스레드가 수행할 함수
//void * : Java의 Object개념, 모든 타입으로 형변환 가능
int main(){
	int status;
	pthread_t thread_id;

	status = pthread_create(&thread_id, NULL, func_phtread, NULL);
					//  스레드 야이디 스레드속성, 함수, 함수의 인자
	if(status !=0){
		printf("[ERR} thread create fail\n");
		exit(1);
	}
	printf("[INFO] %x is created\n", thread_id);
	pthread_join(thread_id, (void **)&status);
	//sleep(5);
	printf("main function end\n");

}

void *func_phtread(void *argv){
	int i = 0;
	while(i<10){
		sleep(1);
		i++;
		printf("thread %dth executing... \n", i);
	}
}
