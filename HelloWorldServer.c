#include <stdio.h>
#include <malloc.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[1024] = "Hi, I'm server\n";
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char *send_buffer = (char*)malloc(sizeof(char)*100);
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
				token = strtok(rcvBuffer, " ");
				strcpy(buffer,"");
				do{
					FILE *fp;
					char fileBuffer[255];
					token = strtok(NULL, " ");
					if(token == NULL)break;
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
				}while(token!=NULL);
			}
			else if(strncasecmp(rcvBuffer, "exec ",5) == 0){
				char *token;
				strcpy(command,"");
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
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}
