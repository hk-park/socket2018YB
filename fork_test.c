#include <stdio.h>
	
	main( )
	{
	   int	pid;
	   int	a = 10;
	   int	b = 20;
	
	   a = a + 1;
	
	   pid = fork( );//자식프로세스를 생성하고,자식프로세스 아이디를 반환
	   if(pid > 0) {//부모프로세스
		a = a + 1;
		printf("[PARENT]\n");
		sleep(10);
		
	   } else if(pid == 0) {//자식프로세스
		b = b + 1;
		printf("[CHILD]\n");
		sleep(30);
	   }	  
	    printf("[CHILD] a=%d/b=%d\n",a,b);
	
	  return 0;
	}
	

