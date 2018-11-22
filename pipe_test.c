#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
 int main(){
	int fd[2], fd2[2];
	char buf[255];
	int pid;
 	if(pipe(fd) < 0 || pipe(fd2) < 0){
		printf("[ERROR] pipe error\n");
		exit(0);
	}
	pid = fork();
	if(pid > 0){
		memset(buf, 0x00, 255);
		read(fd[0], buf, sizeof(buf));
		printf("[PARENT] child message: %s\n", buf);
		memset(buf, 0x00, 255);
		sprintf(buf, "[%d] Hello, I'm parent.", getpid());
		write(fd2[1], buf, strlen(buf));
	}else if (pid == 0){
		memset(buf, 0x00, 255);
		sprintf(buf, "[%d] Hello, I'm child.", getpid());
		write(fd[1], buf, strlen(buf));
		read(fd2[0], buf, sizeof(buf));
		printf("[CHILD] parent message: %s\n", buf);
		
	}else{
		printf("[ERROR] fork() failed\n");
		exit(0);
	}
}
