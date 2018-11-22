#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
int main(){
	int fd[2];//pipe로 사용할때 파일디스크립터
	char buf[255];
	int pid;

	if(pipe(fd) <0  ){//pipe생성
	printf("[ERROR] pipe error\n");
	exit(0);
		
	}
	pid = fork();
	if(pid >0){
	memset(buf , 0x00, 255);
	read(fd[0], buf,sizeof(buf));
	printf("[PARENT] child message %s\n ",buf);
	
	//부모 프로세스
	}//자식프로세스가 파이프를 통해 전달하는 값을 출력
	else if(pid==0){
	memset(buf,0x00,255);
	sprintf(buf ,"[%d]Hello Im client ",getpid());
	write(fd[1], buf, strlen(buf));
//	write(c_socket , buf ,strlen(buf))
	//자식 프로세스
	//파이프를통해 부모프로세스에게 값을 전달
	}else{
		printf("[ERROR] fork() failed\n");
	}

}
