#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(){
	int c_fd_p[2]; // pipe 로 사용할 파일 디스트립터  (양 쪽을 위해 배열 2 )
	int p_fd_c[2];
	char buf[255];
	char buf2[255];
	int pid;

	if(pipe(c_fd_p) < 0) {  //파이프 제작에 실패할 경우 0보다 작은 숫자가 반환됨.
		printf("[ERROR]파이프 에러");
		exit(0); // 프로그램 종료	
	}

	if(pipe(p_fd_c) < 0) {  //파이프 제작에 실패할 경우 0보다 작은 숫자가 반환됨.
		printf("[ERROR]파이프 에러");
		exit(0); // 프로그램 종료	
	}
	pid = fork();
	if( pid > 0 ) //부모
	{	
		
		memset(buf,0x00,sizeof(buf));
		read(c_fd_p[0], buf,sizeof(buf));
		memset(buf2,0x00,sizeof(buf2));

		sprintf(buf2,"[%d]Hello, I'm Parent\n",getpid());


		write(p_fd_c[1],buf2,strlen(buf2));
		printf("[PARENT] child message %s\n",buf);
		//자식 프로세스가 파이프를 통해 전달하는 값을 출력
	}
	else if(pid == 0) {
		//자식
		//파이프를 통해 부모 프로세스에게 값을 전달
		memset(buf,0x00,sizeof(buf));
		sprintf(buf,"[%d] Hello, I'm Child\n", getpid()); 
		write(c_fd_p[1], buf,strlen(buf));

		memset(buf2,0x00,sizeof(buf2));
		read(p_fd_c[0],buf2,sizeof(buf2));
		printf("[Child] Parent message %s\n",buf2);



	
	}else{
		printf("[error] fork() 실패");
	}
	
}
