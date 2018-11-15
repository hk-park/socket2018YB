#include<stdio.h>
main()
{
	int pid;
	int a = 10;
	int b = 20;
	
	a = a + 1;	
	
	pid = fork();  //자식프로세스 저장, 자식프로세스의 프로세스 아이디 반환

		if(pid > 0){	// 부모 프로세스
			a = a + 1;
			printf("[PARENT] a = %d, b = %d\n",a,b);	
		}
		else if(pid == 0){  //자식 프로세스
			b = b + 1;
			printf("[CHILD] a = %d, b = %d\n",a,b);
	}
	return 0;
}
