#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main(){
			int fd[2];//pipe로 사용할 파일디스크립터
			int fd1[2];
			char buf[255];
			char buf1[255];
			int pid;

			if(pipe(fd)<0){ //pipe 생성,생성 실패시 프로그램 종료
					printf("[ERROR] pipe error\n");
					exit(0);
			}
			pid=fork();
			if(pid > 0){
					//부모 프로세스 
					//자식프로세스가 파이프를 통해 전달하는 값을 출력
					memset(buf, 0x00, 255);
					read(fd[0],buf,sizeof(buf));
					printf("[PARENT] child message: %s\n",buf);
			}else if(pid==0){
				//자식 프로세스
				////파이프를 통해 부모 프로세스에게 값을 전달
				memset(buf,0x00,255);
				sprintf(buf,"[%d] Hello, I'm Child.",getpid());
				write(fd[1],buf,strlen(buf));
				
			}else{
						printf("[ERROR] fork() failed \n");
						exit(0);
			}

			if(pipe(fd1)<0){ //pipe 생성,생성 실패시 프로그램 종료
					printf("[ERROR] pipe error\n");
					exit(0);
			}

			pid=fork();
			if(pid > 0){
					//부모 프로세스 
					//자식프로세스가 파이프를 통해 전달하는 값을 출력
					memset(buf1, 0x00, 255);
					sprintf(buf1,"[%d] Hello, I'm Parent.",getpid());
					write(fd1[1],buf1,strlen(buf1));
			}else if(pid==0){
				//자식 프로세스
				////파이프를 통해 부모 프로세스에게 값을 전달
				memset(buf1,0x00,255);
				read(fd1[0],buf1,sizeof(buf1));
				printf("[CHILD] parent message: %s\n",buf1);
				
			}else{
						printf("[ERROR] fork() failed \n");
						exit(0);
			}
}
