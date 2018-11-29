#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void *func_pthread(void *); //thread가 수행할 함수
//void(*) java의 object 비슷 모든걸로 형변환이가능하다.
//
int main()
{
	int status;
	pthread_t thread_id;
	
	status=pthread_create(&thread_id, NULL, func_pthread, NULL);
	//3번째인자는 수행할수이름 4번째인자는3번째인자함수의 인잣값
	if(status != 0){ //성공할시 0return
		printf("[EROOR] thread create failed\n");
		exit(1);
	} //eroor발생할시 보통은 exit(1)
	//exit(0) 과 (1)의 차이는?
	printf("thread %x is create\n",thread_id);
	pthread_join(thread_id, (void **)&status);
//	sleep(5);
	printf("main function end \n");

}
void *func_pthread(void * argv){
	int i=0;
	while(i<10){
	sleep(1);
	i++;
	printf("thread %dth executing ....\n",i);
	}
}
