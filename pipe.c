#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main(){
	int fd[2];
	char buf[255];
	int pid;

	if(pipe(fd) <0){
		printf("[error] pipe error\n");
		exit(0);
	}
	pid = fork();
	if(pid > 0){
		//부모프로세스 
		//자식프로세스가 파이프를 통해 전달하는 값을 출력
		memset(buf.0x00,255);
		read(fd[0]. buf,sizeof(buf));
		printf("[PARENT] child message  %s,buf));
		else if(pid == 0){
		//자식프로세스
		//파이프를 통해 부모  프로세스에 값을 전달 
		memset(buf,0x00,255);
		sprintf(buf,"[%d] hello. i'm client".getpid());
		write(fd[1],buf,strlen(buf));
	
		}else{
	printf("[error] fork() failed");
	exit(0);
}
}
}
