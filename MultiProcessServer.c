#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
#define KILL "Kill"
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "Hi, I'm server\n";
int numClient = 0;
int fd[2];
void sig_handler();
void do_service(int c_socket);
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len;
	int pid;
	signal(SIGCHLD, sig_handler);

	if(pipe(fd) < 0 ){
		printf("[ERR] pipe error\n");
		exit(0);
	}

 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
 
	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}
 
	if(listen(s_socket, 5) == -1) {
		printf("listen Fail\n");
		return -1;
	}
 	
	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		numClient++;
		printf("현재 %d개의 클라이언트가 접속하였습니다.\n", numClient);
		if((pid = fork()) > 0){ //부모
			close(c_socket);
			continue;
		}
		else if(pid == 0){ //자식
			close(s_socket);
			printf("Client is connected\n");
			do_service(c_socket);
			exit(0);
		}
		else{
			printf("fork error\n");
		}
	}
	close(s_socket);	
}

void do_service(int c_socket){
	int rcvLen;
	int   n;
	char rcvBuffer[100], rcv[100], *tok, *tok2, pipeBuf[100];

	while(1){
		rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
		rcvBuffer[rcvLen] = '\0';
		rcv[rcvLen] = '\0';
		printf("[%s] received\n", rcvBuffer);

		if(strncasecmp(rcvBuffer, "kill server", 11) == 0){
			int serverpid = getppid();
			sprintf(pipeBuf, "%d", serverpid);
			write(fd[1],pipeBuf,strlen(pipeBuf));
		}
		else{
			write(fd[1],KILL,sizeof(KILL));
		}
		if(strcmp(rcvBuffer, "안녕하세요") == 0){
			strcpy(rcv, "안녕하세요 만나서 반가워요.");
			n = strlen(rcv);
			write(c_socket, rcv, n);
		}
		else if(strcmp(rcvBuffer, "이름이뭐야?") == 0){
			strcpy(rcv, "내이름은 전진수야.");
			n = strlen(rcv);
			write(c_socket, rcv, n);
		}
		else if(strcmp(rcvBuffer, "몇살이야?") == 0){
			strcpy(rcv, "나는 29살이야.");
			n = strlen(rcv);
			write(c_socket, rcv, n);
		}
		else if(strncasecmp(rcvBuffer, "strlen ", 7) == 0){
			strtok(rcvBuffer, " ");
			tok = strtok(NULL, " ");
			sprintf(rcv, "%d\n", strlen(tok));
			n = strlen(rcv);
			write(c_socket, rcv, n);
		}
		else if(strncasecmp(rcvBuffer, "strcmp ", 7) == 0){
			strtok(rcvBuffer, " ");
			tok = strtok(NULL, " ");
			tok2 = strtok(NULL, " ");
			if(!strcmp(tok, tok2)){
				sprintf(rcv, "%d\n", strcmp(tok, tok2));
			}
			else{
				strcpy(rcv, "두개의 값이 틀립니다!");
			}
			n = strlen(rcv);
			write(c_socket, rcv, n);
		}
		else if(strncasecmp(rcvBuffer, "readfile ", 9) == 0){
			FILE *fp;
			char buffer[100];
			strtok(rcvBuffer, " ");
			tok = strtok(NULL, " ");
			fp = fopen(tok, "r");
			if(fp){
				while(fgets(buffer, 100, (FILE *) fp))
				write(c_socket, buffer, strlen(buffer));
			}
		}
		else if(strncasecmp(rcvBuffer, "exec ", 5) == 0){
			strtok(rcvBuffer, " ");
			tok = strtok(NULL, "\0");
			int ret = system(tok);
			char *ok="command Success!!";
			char *no="command Failed!";
			if(!ret) {
				write(c_socket, ok, strlen(ok));
				write(c_socket, tok, strlen(tok));
			}
			else{
				write(c_socket, no, strlen(no));
			}
		}
		else{
		n = strlen(buffer);
		write(c_socket, buffer, n);
		}
	}
	close(c_socket);
}

void sig_handler(int signo){
	int pid, status;
	char pBuf[100];
	pid = wait(&status);
	printf("1개의 클라이언트가 종료되어 %d개의 클라이언트가 접속되어있습니다.\n",--numClient);
	printf("[pid] = [%d] is terminated. Status = %d\n\n", pid, status);
	read(fd[0], pBuf, sizeof(pBuf));
	if(strcmp(pBuf,KILL) != 0)	
		kill(atoi(pBuf), 9);
}
