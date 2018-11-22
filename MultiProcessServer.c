#include <stdio.h>
#include <malloc.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정
#define PORT 9000
//포트번호 정의

void sig_handler();
int do_service(int c_socket);
int connected = 0;
int fd[2];
int s_socket;

// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
char buffer[1024] = "Hi, I'm server\n";
int main( )
{
	int c_socket;
	struct sockaddr_in s_addr, c_addr;
	int len;
	signal(SIGCHLD, sig_handler);
	int n;
	int rcvLen;//rcv크기
	char *send_buffer = (char*)malloc(sizeof(char)*100);
	char rcvBuffer[100];//rcv버퍼


	if(pipe(fd)<0){
		printf("[ERROR] pipe error");
		exit(1);
	}
	
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
/* PF_INET :IPv4 인터넷 프로토콜 페밀리
 * SOCK_STREAM : 스트림 소캣
 * 0 : 디폴트 프로토콜 사용 TCP or UDP
 */

	memset(&s_addr, 0, sizeof(s_addr));
	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}
/* s_scoket : 서버 소켓
 * (struct sockaddr *) &s_addr :서버 정보
 * sizeof(s_addr) : sockaddr 메모리 크기
 */

	if(listen(s_socket, 5) == -1) {
		printf("listen Fail\n");
		return -1;
	}
/* s_socket :서버 소캣
 * 최대 클라이언트
 */

	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		printf("Client is connected\n");
		connected++;
		printf("현재 %d개의 클라이언트가 접속했습니다.\n", connected);
		int pid = fork();
		if(pid>0){
			close(c_socket);
			continue;
		
		}
		else if(pid == 0){
			close(s_socket);
			do_service(c_socket);
			exit(0);
		}
		else{
			printf("[ERROR] fork() error\n");
		}
		
	}
	close(s_socket);
}
int do_service(int c_socket){
	//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		char rcvBuffer[100];//rcv버퍼
		int rcvLen;//rcv크기
	while(1){
		rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer)); //c_socket수신버퍼에서 len마큼 rcvBuffer로 가져오다.
		rcvBuffer[rcvLen] = '\0'; //마지막 문자는 NULL문자로
		printf("[%s] received\n", rcvBuffer);
		if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
			break;
		else if(strcmp(rcvBuffer,"안녕하세요") == 0){
			strcpy(buffer ,"안녕하세요. 만나서 반가워요.");
		}
		else if(strcmp(rcvBuffer,"이름이 뭐야?") == 0){
			strcpy(buffer, "내 이름은 이선룡이야");
		}
		else if(strcmp(rcvBuffer,"몇살이야?") == 0){
			strcpy(buffer, "나는 24살이야");
		}
		else if(!strncasecmp(rcvBuffer, "strlen ", strlen("strlen "))){
			sprintf(buffer,"내 문자열의 길이는 %d입니다", strlen(rcvBuffer) - strlen("strlen "));
		}
		else if(strncasecmp(rcvBuffer, "strcmp ", 7) == 0){
			char *token;
			char *str[3];
			int i=0;
			token = strtok(rcvBuffer, " ");

			while(token != NULL){
				str[i++] = token;
				token = strtok(NULL, " ");
			}
			if(i<3)
				sprintf(buffer, "두개의 문자열을 입력해주세요");
			else
				sprintf(buffer, "%d\n", strcmp(str[1], str[2]));
		}
		else if(strncasecmp(rcvBuffer, "fileread ",7) == 0){
			char *token;
			char *commands;
			token = strtok(rcvBuffer, " ");
			commands = strtok(NULL,"\0");
			token = strtok(commands," ");
			strcpy(buffer,"");
			while(token!=NULL){
				FILE *fp;
				char fileBuffer[255];
				fp = fopen(token,"r");
				if(fp){
					while(fgets(fileBuffer,255,fp)){
						strcat(buffer,fileBuffer);
					}
				}
				else{
					sprintf(fileBuffer,"%s 파일을 여는데 실패했습니다.\n",token);
					strcat(buffer,fileBuffer);
				}
				token = strtok(NULL, " ");
			}
		}
		else if(strncasecmp(rcvBuffer, "exec ",5) == 0){
			char *token;
			token = strtok(rcvBuffer, " ");
			token = strtok(NULL, "\0");
			system(token);
			strcpy(buffer,token);
		}
		else {
			sprintf(buffer ,"다시 입력해 주세요");
		}
		int n = strlen(buffer);
		write(c_socket, buffer, n);
	}
	close(c_socket);

	write(fd[1],rcvBuffer,rcvLen);
}

void sig_handler(){
	int pid;
	int status;
	char rcvBuffer[1000];

	pid = wait(&status);
	read(fd[0],rcvBuffer,sizeof(rcvBuffer));
	printf("PID : %d is terminated Status = %s\n",pid,status);
	printf("1개의 클라이언트가 접속이 종료되어 %d개의 클라이언트가 접속중입니다.\n",--connected);
	if(strncasecmp(rcvBuffer,"kill server",11)==0){
		printf("[SERVER] server closeing...\n");
		close(s_socket);
		exit(0);
	}
}
