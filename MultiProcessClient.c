#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define PORT 9000
#define BUFSIZ 10000
#define IPADDR "127.0.0.1"
char ss[100];
main()
{
	int c_socket,s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len, n;
	char rcvBuffer[BUFSIZ];
	c_socket = socket(PF_INET, SOCK_STREAM,0);
	memset(&c_addr,0,sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);
	if(connect(c_socket,(struct sockaddr *)&c_addr, sizeof(c_addr))==-1)
	{
		printf("faile");	
		close(c_socket);
		return -1;
	}
	if((n = read(c_socket,rcvBuffer,sizeof(rcvBuffer)))<0)return -1;
	rcvBuffer[n] = '\0';
	printf("%s\n",rcvBuffer);

	int fd[2];
	char buf[255];
	if(pipe(fd) < 0){//생성실패시 프로그램 종료
		printf("[ERROR] pipe error\n");
		exit(0);
	}
	int pid = fork();
	if(pid > 0){

		while(1){
			n=read(c_socket,rcvBuffer,sizeof(rcvBuffer));
			if(n<0){
				printf("error\n");
				break;
			}else if(n>=0){
				rcvBuffer[n] = '\0';
				if(strncmp(ss,"kill server",strlen("kill server"))==0){
					break;
				}
				printf("%s\n",rcvBuffer,n);
			}

		}
	}
	else if(pid == 0){//자식

		pid = getppid();
//		printf("my : %d\nsub : %d",getpid(),pid);
		while(1){
			fgets(ss,sizeof(ss),stdin);
			if(strncmp(ss,"quit",strlen("quit"))==0 || strncmp(ss,"kill server",strlen("kill server"))==0){
				break;
			}
			ss[strlen(ss)-1] = '\0';
			write(c_socket,ss,strlen(ss));
		}
	}
	else{
		printf("[ERROR] fork() failed\n");
		exit(0);
	}
	kill(pid,SIGKILL);
	close(c_socket);
	exit(0);
//	return 1;
}
