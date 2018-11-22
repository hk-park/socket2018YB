#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>



int main(){
	int fd[2];
	char buf[255];
	int pid;

	if(pipe(fd)<0){
		printf("[ERROR] pipe error\n");
		exit(1);
	}

	pid = fork();

	if(pid>0){ //부모 프로세스
		memset(buf,0,255);
		read(fd[0],buf,sizeof(buf));
		printf("[PARENT] child message :%s\n",buf);
	}
	else if(pid == 0){ //자식 프로세스
		memset(buf,0,255);
		sprintf(buf,"[%d] Hello, I'm child",getpid());
		write(fd[1],buf,strlen(buf));//파이프로 값 전달
	}
	else{
		printf("[ERROR] fork() error\n");
	}


	return 0;
}

