#include <stdio.h>

main(){
	int pid;
	int a = 10;
	int b = 20;

	a = a+1;

	pid = fork();

	if(pid>0){
		a = a+1;
		printf("[PARENT] a = %d b = %d\n",a,b);
	}
	else{
		printf("[CHILD] a = %d b = %d\n",a,b);
	}
	return 0;
}
