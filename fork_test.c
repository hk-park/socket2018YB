#include<stdio.h>

int main()
{
	int pid;
	int a = 10;
	int b = 20;

	pid = fork();
	if(pid > 0){
		a = a+1;
		printf("[PARNET] a = %d, b = %d\n",a,b);
	}else if(pid == 0){
		b = b+1;
		printf("[CHILD] a = %d , b = %d\n",a,b);
	}


}
