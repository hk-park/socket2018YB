#include <stdio.h>

int main(){
	int pid;
	int a = 10;
	int b = 20;
	
	a = a+1;
	
	pid = fork();
	if(pid > 0){ //부모 프로세스
		a = a+1;
		printf("PARENT\n");
	}else{
		b = b+1;
		printf("CHILD\n");
	}
	printf("[%d] a = %d, b = %d\n", getpid(), a, b );
	return 0;
}
