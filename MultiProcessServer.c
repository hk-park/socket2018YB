#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

#define PORT 9000
#define BUFSIZE 10000

void do_service(int c_socket);

char buffer[BUFSIZE] = "Hi, I'm server\n";
char rcvBuffer[BUFSIZE];
char tempBuffer[BUFSIZE];
char clearBuffer[BUFSIZE];

int pid;

char *token;
char *str[5];
int icount=0;

int len, n;
int rcvLen;
FILE *fp;

int main(){
	int c_socket, s_socket;
	//s_socket: 클라이언트의 접속을 기다리는 소켓
	//c_socket: 클라이언트에게 메시지를 전달하기 위한 소켓
	struct sockaddr_in s_addr, c_aadr;
	
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
		if((pid=fork())>0){
			close(c_socket);
			continue;
		}else if(pid == 0){
			close(s_socket);
			do_service(c_socket);
			exit(0);
		}else{
			//에러 처리
		}
	}
	close(s_socket);
}

void do_service(int c_socket){
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
			token = strtok(rcvBuffer, " ");
			while(token != NULL){
				str[icount++]=token;
				token = strtok(NULL, " ");
			}
			if(icount<=2 || icount>3){
				sprintf(buffer, "문자열 비교를 위해서는 두 문자열이 필요합니다.\n");
				icount=0;
			}
			else if(strcmp(str[1], str[2]) == 0){
				sprintf(buffer, "%s %s 같음\n", str[1], str[2]);
				icount=0;
			}
			else if(strcmp(str[1], str[2]) >= 1){
				sprintf(buffer, "%s %s +다름\n", str[1], str[2]);
				icount=0;
			}
			else if(strcmp(str[1], str[2]) <= -1){
				sprintf(buffer, "%s %s -다름\n", str[1], str[2]);
				icount=0;
			}
		}
		else if(!strncasecmp(rcvBuffer,"readfile", strlen("readfile"))){
			token = strtok(rcvBuffer, " ");
			while(token != NULL){
				str[icount++]=token;
				token = strtok(NULL, " ");
			}
			if(icount >= 2){
				if((fp = fopen(str[1], "r")) == NULL){
					strcpy(buffer,"없는 파일 입니다.\n");
					icount=0;
				}
				else{
					while(fgets(buffer, BUFSIZE, (FILE *)fp)){
						strcat(tempBuffer, buffer);
						printf("클라이언트 전송 완료\n");
					}
					strcpy(buffer, tempBuffer);
					strcpy(tempBuffer, clearBuffer);
					fclose(fp);
					icount=0;
				}
			}
			else{
				sprintf(buffer, "잘못 입력 되었습니다.\n");
				icount=0;
			}
		}
		else if(!strncasecmp(rcvBuffer,"exec", strlen("exec"))){
			int tf;
			token = strtok(rcvBuffer," ");
			while(token!=NULL){
				str[icount] = token;
				if(icount>=1){
					strcat(tempBuffer, str[1]);
				}
				token=strtok(NULL,"\0");
				icount++;
			}
			tf = system(tempBuffer);
			icount=0;
			if(tf == 0){
				sprintf(buffer, "[%s]command is exectued.\n",tempBuffer);
				strcpy(tempBuffer,clearBuffer);
			}
			else{
				sprintf(buffer, "[%s]command failed.\n",tempBuffer);
				strcpy(tempBuffer,clearBuffer);
			}
		}
		else {
			strcpy(buffer, "무슨말이오.??\n");
		}
		/*if(strncasecmp(rcvBuffer, "kill server", 11) == 0){
			break;
		}*/
		n = strlen(buffer);
		write(c_socket, buffer, n);
	}

	write(c_socket, buffer, strlen(buffer));
	close(c_socket);
	/*if(!strncasecmp(rcvBuffer, "kill server", 11)){
		break;
	}*/
}
