#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

main() {
	int fd[2], fd2[2];				// 파이프로 사용할 디스크럽터
	char buf[255];
	int pid;

	if(pipe(fd) < 0  || pipe(fd2) < 0) {			// pipe 생성, 생성 실패 시 프로그램 종료
		printf("[ERROR] pipe erroe\n");
		exit(0);
	}
	pid = fork();
	if(pid > 0) {				// 부모 프로세스, 파이프를 통해 자식이 전달하는 값 출력
		memset(buf, 0x00, 255);
		read(fd[0], buf, sizeof(buf));
		printf("[PARENT] Child message : %s\n", buf);
		memset(buf, 0x00, 255);
		sprintf(buf, "[%d] Hello, I'm Parent.", getpid());
		write(fd2[1], buf, strlen(buf));
	}
	else if(pid == 0) {			// 자식 프로세스, 파이프를통해 부모에게 값 전달
		memset(buf, 0x00, 255);
		sprintf(buf, "[%d] Hello, I'm Child.", getpid());
		write(fd[1], buf, strlen(buf));
		read(fd2[0], buf, sizeof(buf));
		printf("[CHILD] Parent message : %s\n", buf);
	}
	else {
		printf("[ERROR] fork() failed\n");
		exit(0);
	}
}
		
