#include <stdio.h>

int main(){
	int pid;//pid : 리눅스에서 제공,프로세스 아이디
	int a=10;
	int b=20;

	a=a+1;

	pid=fork();//자식 프로세스 생성하고 자식프로세스의 프로세스아이디를 반환
	///fork()이후에는 항상 동일한 형태로 나옴
	if(pid > 0){//부모 프로세스
		a=a+1;
		printf("[PARENT]a = %d, b=%d\n",a,b);		
		
	}else{//자식프로세스(pid=0)
	//좀 더 명확하게 하려면 0인가 0보다 작은지 여부 확인필요
		b=b+1;
		printf("[CHILD]a = %d, b = %d\n",a,b);
	}
	return 0;
}
