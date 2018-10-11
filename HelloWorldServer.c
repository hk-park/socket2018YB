#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> 
#include <stdlib.h> 
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000

// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
 
#define PORT 9000
#define BUFSIZE 1000
#define SLEEPTIME 1
char buffer[BUFSIZE] = "hello, I'm server";
char rcvBuffer[BUFSIZE];

void do_service(int c_socket);

main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char buffer[100] = "Hi, I'm server\n";
	char rcvBuffer[100];
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
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
		printf("Client is Connected\n");
		do_service(c_socket);
		//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			else if(strncasecmp(rcvBuffer, "hello", 5) == 0)
				strcpy(buffer, "Hi, Nice to meet you");
			else if(strncasecmp(rcvBuffer, "what is your name", 17) == 0)
				strcpy(buffer, "my name is soyeon");
			else if(strncasecmp(rcvBuffer, "how old are you", 15) == 0)
				strcpy(buffer, "I am 22 old");
			n = strlen(buffer);
			write(c_socket, buffer, n);
		}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}

void do_service(int c_socket){
	int   n;
	int readSize;
	char *token;
	char tempBuf[BUFSIZE];
	while(1){
		memset(rcvBuffer, 0, BUFSIZE);
		if((readSize = read(c_socket, rcvBuffer, sizeof(rcvBuffer))) < 0) {
       	 		break;
		}
		printf("Received Data From Client: %s\n", rcvBuffer);
		if(strcasecmp(rcvBuffer, "quit") == 0){
			break;
		}else if(strcasecmp(rcvBuffer, "안녕하세요") == 0){
			strcpy(buffer, "반갑습니다");
		}else if(strcasecmp(rcvBuffer, "이름이 뭐야?") == 0){
			strcpy(buffer, "김소연");
		}else if(strcasecmp(rcvBuffer, "몇 살이야?") == 0){
			strcpy(buffer, "22살");
		}else if(strncasecmp(rcvBuffer, "strlen ", 7) == 0){
			token = strtok(rcvBuffer, " "); //token = strlen
			token = strtok(NULL, " "); //token = 길이를 구하고자 하는 문자열
			strcpy(tempBuf, token);
			sprintf(buffer, "%s 문자열의 길이는 %d입니다.", tempBuf, strlen(tempBuf)); 				
		}else if(strncasecmp(rcvBuffer, "strcmp ", 7) == 0){
			char str[3][BUFSIZE];
			int i = 0;
			token = strtok(rcvBuffer, " "); //token = strcmp
			while(token != NULL){
				strcpy(str[i], token);
				i++;
				token = strtok(NULL, " ");
			}
			if(strcasecmp(str[1], str[2])==0){
				sprintf(buffer, "%s와 %s는 같은 문자열입니다.", str[1], str[2]);
			}else{
				sprintf(buffer, "%s와 %s는 다른 문자열입니다.", str[1], str[2]);
			}
		}else if(strncasecmp(rcvBuffer, "readfile", 8) == 0){
			token = strtok(rcvBuffer, " ");
			token = strtok(NULL, " ");
			printf("readfile: %s\n", token); 
			FILE *fp = fopen(token, "r");
			char tempStr[BUFSIZE];
			memset(buffer, 0, BUFSIZE);
			if(fp){
				while(fgets(tempStr, BUFSIZE, (FILE *)fp)){
					strcat(buffer, tempStr);
				}
				fclose(fp);
			}else{
				strcpy(buffer, "cannot find file");
			}

		}else if(strncasecmp(rcvBuffer,"exec", 4)==0){
			char *command;
			token = strtok(rcvBuffer, " ");
			command= strtok(NULL, "\0");
			printf("command : %s\n", command);
			int result = system(command);
			printf("result: %d\n", result);
			if(result){
				sprintf(buffer, "[%s] command failed", command);
			}else{
				sprintf(buffer, "[%s] command is executed", command);
			}
			 
		} else{
			strcpy(buffer, "뭐라는건지 모르겠음");
		}
		n = strlen(buffer);
		sleep(SLEEPTIME);
		write(c_socket, buffer, n);
		printf("Send Data: %s\n", buffer);
		//write(c_socket, rcvBuffer, strlen(rcvBuffer));
		//printf("Send Data: %s\n", rcvBuffer);
	}
	close(c_socket);
}
