#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
	int fd1[2];	//pipe로 사용할 파일디스크립터(자식->부모 파이프)
	int fd2[2];	//부모->자식 파이프
	char buf[255];
	int pid;

	if(pipe(fd1) || pipe(fd2) < 0) {	//pipe 생성. 생성 실패 시, 프로그램 종료.
		printf("[ERROR] pipe error.\n");
		exit(0);
	}

	pid = fork();
	
	if(pid > 0){
		//부모 프로세스
		//자식 프로세스가 파이프를 통해 전달하는 값을 출력
		memset(buf, 0x00, 255);
		read(fd1[0], buf, sizeof(buf));
		printf("[PAPENT] child message: %s", buf);

		sprintf(buf, "[%d] Hi, I'm your father0_0\n", getpid());
		write(fd2[1], buf, strlen(buf));
	}	
	else if(pid==0){
		//자식 프로세스
		//파이프를 통해 부모 프로세스에게 값을 전달
		memset(buf, 0x00, 255);
		sprintf(buf, "[%d] Hello, I'm your child^^\n", getpid());
		write(fd1[1], buf, strlen(buf));
		
		read(fd2[0], buf, sizeof(buf));
		printf("[PAPENT] father's message: %s", buf); 
	}
	else{
		printf("[ERROR] fork() failed.\n");
		exit(0);
	}
}
