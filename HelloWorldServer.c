#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "Hi, I'm server\n";
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char rcvBuffer[100], rcv[100], *tok, *tok2;
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
		//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			rcv[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			

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
			else{
			n = strlen(buffer);
			write(c_socket, buffer, n);
			}
		}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}
