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
	char rcvBuffer[100];
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
			printf("[%s] received\n", rcvBuffer);
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			if(strncasecmp(rcvBuffer, "안녕하세요",5) == 0){
				printf("안녕하세요, 나도 만나서 반가워요.\n");
			}
			if(strncasecmp(rcvBuffer, "이름이 뭐야?",7)==0){
			  printf("내 이름은 AAA야\n");
			}
			if(strncasecmp(rcvBuffer, "몇 살이야?",6)==0){
				printf("나는 50살이야\n");
			}
		  if(strncasecmp(rcvBuffer, "strlen",6)==0){
				printf("길이는 %d\n",strlen(rcvBuffer)-7);			
			}
		  if(strncasecmp(rcvBuffer,"strcmp",6)==0){
				
				char*token;
				token = strtok(rcvBuffer,"");
				while(token != NULL){
					token = strtok(NULL,"");
				}
	
			}
				
		}
			n = strlen(buffer);
			write(c_socket, buffer, n);
			}
			close(c_socket);
			if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}
