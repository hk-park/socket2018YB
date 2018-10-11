#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
//char buffer[100] = "hello world\n";
char buffer[BUFSIZ] = "Hi, I'm server\n";

int main(){
	int c_socket, s_socket;
	//s_socket: 클라이언트의 접속을 기다리는 소켓
	//c_socket: 클라이언트에게 메시지를 전달하기 위한 소켓
	struct sockaddr_in s_addr, c_aadr;
	int len, n;
	int rcvLen;
	char rcvBuffer[BUFSIZ];
	char tempBuffer[BUFSIZ];
	FILE *fp;
	/*if(fp){
		while(fgets(buffer, BUFSIZ, (FILE *)fp)){
			printf("%s\n", buffer);
		}
		fclose(fp);
		int ret = system("mkdir testdir");
		if(!ret){
			printf("Command Success!!\n");
		}
		else{
			printf("Command Failed!!\n");
		}
	}*/
	
	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);//IP주소 저장
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
	
	if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1){
		printf("[ERR] Cannot bind\n");
		return -1;
	}
	
	if(listen(s_socket, 5) == -1){
		printf("[ERR] Cannot listen\n");
		return -1;
	}
	
	while(1){
		len = sizeof(c_aadr);
		c_socket = accept(s_socket, (struct sockaddr *)&c_aadr, &len);
		printf("Client is connected\n");
			while(1){
				rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
				rcvBuffer[rcvLen]='\0';
				printf("[%s] received\n",rcvBuffer);
				if(!strncmp(rcvBuffer, "안녕하세요.", strlen("안녕하세요."))){
					strcpy(buffer, rcvBuffer);
					strcat(buffer, "만나서 반가워요.");
				}
				else if(!strncmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?"))){
					strcpy(buffer, "내이름은 xxx야.");
				}
				else if(!strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?"))){
					strcpy(buffer, "나는 xx살이야.");
				}
				else if(!strncasecmp(rcvBuffer,"strlen", 6)){
					sprintf(buffer,"내 문자열의 길이는 %d입니다.\n", strlen(rcvBuffer)-7);
				}
				else if(!strncasecmp(rcvBuffer,"strcmp", 6)){
					char *token;
					char *str[3];
					int i=0;
					token = strtok(rcvBuffer, " ");
					while(token != NULL){
						str[i++]=token;
						token = strtok(NULL, " ");
					}
					if(i<=1 || i>=3){
						sprintf(buffer, "문자열 비교를 위해서는 두 문자열이 필요합니다.\n");
					}
					else if(strcmp(str[1], str[2]) == 0){
						sprintf(buffer, "%s %s 같음\n", str[1], str[2]);
					}
					else if(strcmp(str[1], str[2]) >= 1){
						sprintf(buffer, "%s %s +다름\n", str[1], str[2]);
					}
					else if(strcmp(str[1], str[2]) <= -1){
						sprintf(buffer, "%s %s -다름\n", str[1], str[2]);
					}
				}
				else if(!strncasecmp(rcvBuffer,"readflie", strlen("readfile"))){
					//sprintf(buffer,"\n파일읽는 중..\n");
					//write(c_socket, buffer, strlen(buffer));
					//rcvBuffer[strlen(rcvBuffer)-1] = '\0';
					
					char *token_2;
					char *str_2[2];
					int i=0;
					token_2 = strtok(rcvBuffer, " ");
					while(token_2 != NULL){
						str_2[i++]=token_2;
						token_2 = strtok(NULL, " ");
					}
					if(i >= 2){
						//sprintf(buffer,"파일읽는 중_2\n");
						//write(c_socket, buffer, strlen(buffer));
						
						fp = fopen(str_2[1], "r");
						while(fgets(buffer, BUFSIZ, (FILE *)fp)){
							strcat(tempBuffer, buffer);
							strcpy(buffer, tempBuffer);
							printf("클라이언트 전송 완료\n");
						}
						fclose(fp);
					}
					else{
						sprintf(buffer, "잘못 입력 되었습니다.\n");
						//write(c_socket, buffer, strlen(buffer));
					}
				}
				else {
					strcpy(buffer, "무슨말이오.??\n");
				}
				if(strncasecmp(rcvBuffer, "kill server", 11) == 0){
					break;
				}
				n = strlen(buffer);
				write(c_socket, buffer, n);
			}
		write(c_socket, buffer, strlen(buffer));
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11)){
			break;
		}
	}
	close(s_socket);
}
