#include <stdio.h>

int main(){
	int pid;
	int a=10;
	int b=20;

	a=a+1;
	
	pid=fork(); //자식 프로세스 생성, 자식 프로세스의 프로세스 아이디를 반환
	if(pid>0){ //부모 프로세스
		a=a+1;
		//printf("PARENT");
		printf("[PARENT] a=%d / b=%d\n", a, b);
	}
	else if(pid==0){ //자식 프로세스. fork() 함수가 성공하면 자식 프로세스의 PID가 부모에게 리턴되며, 자식에게는 0이 리턴
		b=b+1;
		//printf("CHILD");
		printf("[CHILD] a=%d / b=%d\n",a, b);
	}
	//printf("[%d] a=%d / b=%d\n", getpid(), a, b);
}
