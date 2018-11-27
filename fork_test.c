#include <stdio.h>
<<<<<<< HEAD

int main(){
	int pid;
	int a = 10;
	int b = 20;
	
	a = a+1;
	
	pid = fork();
	if(pid > 0){ //부모 프로세스
		a = a+1;
		printf("PARENT\n");
		sleep(30);
	}else{
		b = b+1;
		printf("CHILD\n");
		sleep(10);
	}
	printf("[%d] a = %d, b = %d\n", getpid(), a, b );
	return 0;
}
=======
	
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
	

>>>>>>> f920cda21f3602061253860c88f0dc1a3af40ca3
