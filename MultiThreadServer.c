#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정
#define PORT 9000
//포트번호 정의

// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
char buffer[1024] = "Hi, I'm server\n";
void do_service(void* data);
int client_count = 0;

int main( )
{
	pthread_t pthread;
	int	thr_id;
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;

	char *send_buffer = (char*)malloc(sizeof(char)*100);



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
		thr_id = pthread_create(&pthread, NULL, do_service, (void *)&c_socket);

	}
	close(s_socket);
}


void do_service(void* data){
	int c_socket = *(int *)data;
	int   n;
	int rcvLen;//rcv크기
	char rcvBuffer[100];//rcv버퍼
	//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
	printf("Client is connected\n");
	printf("현재 총 %d개의 클라이언트가 접속중입니다.\n",++client_count);
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
		n = strlen(buffer);
		write(c_socket, buffer, n);
	}
	close(c_socket);
	printf("1개의 클라이언트가 접속을 종료하여 현재 %d개의 클라이언트가 접속중입니다.\n",--client_count);
	if(!strncasecmp(rcvBuffer, "kill server", 11)){
		printf("서버를 종료합니다...\n");
		exit(0);
	}
}
