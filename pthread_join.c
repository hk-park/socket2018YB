#include <stdio.h>
#include <pthread.h>

void *do_sum(void *val); //펙토리얼 출력기 

int main()
{
	int thr_id;
	pthread_t p_thread[3];
	int status;
	int a[3] = { 4, 5, 6 } ;
	int i;
	for(i=0; i<3; i++){
		thr_id = pthread_create(&p_thread[i],NULL, do_sum,(void*)&a[i]);
	//	pthread_join(p_thread[i], (void**)&status);
	} 	//요렇게하면 프로그램이 순서대로 실행된다. 물론 난 이 쓰레드를 동시에 실행시키게 문장을 바꿀 수 있다.
	i=0;	
	while(i<3)
	{
		pthread_join(p_thread[i], (void**)&status);
		i++;
	}	
	printf("main program is ended \n");
	return 0;

}

void *do_sum(void *val)
{
	int i;
	int sum = 0;
	int max =  *((int *)val);  //보이드 포인터에서 값 뽑아내기.

	for(i=0; i< max; i++)
	{
		sum = sum + i;
		printf("%d \n",sum);
		sleep(1);
	}
	printf("recycle times:%d \tsum : %d\n",max, sum);	
}
