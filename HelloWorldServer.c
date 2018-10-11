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
char bufferMsg[100];
 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char rcvBuffer[100], rcvStr[100];
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
			if(!strncmp(rcvBuffer, ("안녕하세요"),strlen("안녕하세요"))){
				strcpy(bufferMsg, "안녕하세요. 만나서 반가워요.\n");
				write(c_socket, bufferMsg, strlen(bufferMsg));
				}
			else if(!strncmp(rcvBuffer, "이름이 머야?",strlen("이름이 머야?"))){
				strcpy(bufferMsg, "내 이름은 시리야.\n");
				write(c_socket, bufferMsg, strlen(bufferMsg));
				}
			else if(!strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?"))){
				strcpy(bufferMsg, "나는 21살이야. \n");
				write(c_socket, bufferMsg, strlen(bufferMsg));
				}
			else if(strncasecmp(rcvBuffer, "strlen ", 7)== 0){
				strtok(rcvBuffer, " ");
				char *rcvTok = strtok(NULL, " ");
				sprintf(rcvStr, "%d\n", strlen(rcvTok));
				write(c_socket, rcvStr, strlen (rcvStr));
   				}
			else if(strncasecmp(rcvBuffer, "strcmp ", 7)== 0){
				strtok(rcvBuffer, " ");
				char *rcvTok1 = strtok(NULL, " ");
				char *rcvTok2 = strtok(NULL, " ");
				int rcvCmp = strcmp(rcvTok1, rcvTok2);
				sprintf(rcvStr, "%d\n", rcvCmp);
				write(c_socket, rcvStr, strlen(rcvStr));
				}
			else if(!strncasecmp(rcvBuffer, "readfile ", 9)) {
				FILE *fp;
				char buffer[100];
				char *token;
				strtok(rcvBuffer, " ");
				token = strtok(NULL, " ");		

    				fp = fopen(token, "r");
    				if(fp) {
					while(fgets(buffer, 100, (FILE *)fp))
       					printf("%s", buffer);
				}
				fclose(fp);
			}
			else if(!strncasecmp(rcvBuffer, "exec ", 5)) {
				char *token;
                                strtok(rcvBuffer, " ");
                                token = strtok(NULL, "\0");
				
				int ret = system(token);
				if(!ret)
                			printf("command Success!\n");
        			else
                			printf("command Failed!\n");
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
