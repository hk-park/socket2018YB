#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
#define BUFSIZE 10000
//#define PORT 10000

// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[BUFSIZE] = "Hi, I'm server\n";
 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int pid;
	int   n;
	int rcvLen;
	char rcvBuffer[BUFSIZE];
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
		if((pid = fork())>0){
		close(c_socket);
		continue;
		}
		else if(pid ==fork){
		close(s_socket);
		do_service(c_socket);
		exit(0);
		}
	}
}	
void do_service(int c_socket){	
//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		
	int  s_socket;
        struct sockaddr_in s_addr, c_addr;
        int   len;
        int pid;
        int   n;
        int rcvLen;
        char rcvBuffer[BUFSIZE];
        s_socket = socket(PF_INET, SOCK_STREAM, 0);

	printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
		/*	if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
			break;*/
			if(!strncmp(rcvBuffer,"안녕하세요", strlen("안녕하세요")))
				strcpy(buffer,"안녕하세요 만나서 반가워요.");
			else if(!strncmp(rcvBuffer,"이름이 머야?", strlen("이름이 머야?")))
				strcpy(buffer,"이재영");
			else if(!strncmp(rcvBuffer,"몇 살이야?", strlen("몇 살이야?")))
				strcpy(buffer,"스무세살이야");
			else if(!strncasecmp(rcvBuffer,"strlen ",7))
				sprintf(buffer,"내 문자열의 길이는 %d입니다",strlen(rcvBuffer)-7);
			else if(!strncasecmp(rcvBuffer,"strcmp ",7)){
				char *token;
				char *str[3];
				int i = 0;
				token = strtok(rcvBuffer," ");
				while(token !=NULL){
					str[i] = token;
					i++;
					token = strtok(NULL," ");
				}
				if(i<3)
					sprintf(buffer,"문자열 비교를 위해서는 두 문자열이필요하다");
				else if(!strcmp(str[1],str[2])) //같은 문자열이면
					sprintf(buffer,"%s 와 %s 는 같은 문자열입니다",str[1],str[2]);
				else
					sprintf(buffer,"%s와 %s는 다른 문자열입니다",str[1],str[2]);
			}
			if(!strncasecmp(rcvBuffer,"readfile ",9)){
				char *token;
				char *str[3];
				int i=0;
				token = strtok(rcvBuffer, " ");
				while(token !=NULL){
				str[i++] = token;
				token = strtok(NULL, " ");
				}
			//str[0] = readfile
			//str[1] = filename
			if(i<2)
				sprintf(buffer, "readfile 기능을 위해서는 readfile <파일명> 형태로입력해주시오");
			FILE *fp = fopen(str[1], "r");
			if(fp){
				char *tempStr[BUFSIZE];
				memset(buffer,0,BUFSIZE);
				while(fgets(tempStr, BUFSIZE, (FILE *)fp))
				{
					strcat(buffer, tempStr);
				}
			fclose(fp);
			}else{
				sprintf(buffer, "파일이 없습니다. ");
			}
			}else if (!strncasecmp(rcvBuffer,"exec ",5)){
				char *command;
				char token;
				token = strtok(rcvBuffer," ");
				command = strtok(NULL , "\0");
				printf("command :%s \n", command);
				int result= system(command);
				if(result)
					sprintf(buffer, "[%s] 명령어가 실패했습니다.",command);
				 else
					sprintf(buffer, "[%s] 명령어가 성공했습니다.",command);
			}else
				strcpy(buffer, "무슨말인지 모르겠습니다.");
				
			n = strlen(buffer);
			write(c_socket,buffer,n);
		
		}
		close(c_socket);
	/*if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;*/
}

