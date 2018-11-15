#include <stdio.h>

int main(){
	int pid;
	int a = 10;
	int b = 20;
	
	a = a+1;
	
	pid = fork(); //자식 프로세스 생성, 자식 PID 반환
	
	if(pid > 0){	//부모 프로세스
		a = a+1;
		printf("[PARENT] a = %d, b = %d\n", a, b);
	}
	else{	//자식 프로세스 (자식은 항상 PID가 0)
		b = b+1;
		printf("[CHILD] a = %d, b = %d\n", a, b);
	}

	return 0;
}
