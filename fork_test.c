#include <stdio.h>

main()
{
	int pid;
	int a=10;
	int b=20;

	a= a+1;

	pid = fork(); //자식 프로세스 생성하고, 자식 프로세스의 아이디를 반환
	//멀티 프로세스는 항상 동일한 형태가 나옴, 
	if(pid > 0 )//부모 프로세스가 해야 할 일
	{
		a = a + 1;
		printf("PARENT");
	}
	else if(pid == 0) //자식 프로세스가 해야 할 일 (pid = 0)
	{
		b = b + 1;
		printf("CHILD");
	}
	printf("[%d} a= %d/b= %d\n", getpid(),a,b);

	return 0;
}
