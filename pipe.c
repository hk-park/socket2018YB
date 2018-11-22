#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main(){
	int fd[2]; //pipe로 사용할 파일디스크립트
	char buf[255];
	int pid;

	//pipe 생성 실패 시 프로그램 종
	if(pipe(fd) <0){료
		printf("[Error] pipe errer!!1\n")l
		exit(1);
	}
	pid=fork();
	if(pid>0){
		//부모 프로세스에서는 자식프로세스가 전달하는 파이프를 통해 전달하는 값을 출력	
		memset(buf, 0x00,255);
		read(fd[0],buf,sizeof(buf));
		printf("[PARENT] 클라이언트 메세지:%s\n",buf);
	}else if(pid==0){
		//자식 프로세스에서는 파이프를 통해 부모 프로세스에게 값을 전달
		memset(buf, 0x00,255);
		sprintf(buf,"[%d] 안녕, 나는 클라이언트야",getpid());
		write(fd[1],buf,strlen(buf));
	}else{
		printf("[Error] fork() failed!!!\n");
		exit(1);
	}
}
