#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main(){
	int fd1[2];
	int fd2[2];
	char buf[255];
	char buf2[255];
	int pid;

	if(pipe(fd1)<0){
		printf("[ERROR] pipe1 error\n");
		exit(0);
	}
	if(pipe(fd2)<0){
		printf("[ERROR] pipe2 error\n");
		exit(0);
	}
	pid = fork();
	if(pid>0){//parent
		memset(buf, 0x00, 255);
		read(fd1[0], buf, sizeof(buf));
		printf("[PARENT] child message : %s\n", buf);
		memset(buf2, 0x00, 255);
		sprintf(buf2, "[%d] Hello, I'm parent.", getpid());
		write(fd2[1], buf2, strlen(buf2));
	}else if(pid ==0){//child
		memset(buf, 0x00, 255);
		sprintf(buf, "[%d] Hello, I'm child.", getpid());
		write(fd1[1], buf, strlen(buf));
		memset(buf2, 0x00, 255);
		read(fd2[0], buf2, sizeof(buf));
		printf("[CHILD] parent message : %s\n", buf2);
	}else{
		printf("[ERROR] fork() failed\n");
		exit(0);
	}
}
