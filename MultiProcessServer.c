#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
#define MAX 100
//#define PORT 10000
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "Hi, I'm server\n";
int i;
int n;
int pid;
int   c_socket, s_socket;
struct sockaddr_in s_addr, c_addr;
int   len;
int rcvLen;
char rcvBuffer[100];
int conn_s;
 main( )
{
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
        memset(&s_addr, 0, sizeof(s_addr));
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
		if((pid=fork()) > 0){
			close(c_socket);
			continue;
		}else if(pid == 0){
			close(s_socket);
			do_service(c_socket);
			exit(0);
		}
	}
}
do_service(int c_socket){
	while(1){
		
                //3-3.클라이언트가 접속했을 때 "Client is connected" 출력
                printf("Client is connected\n");
                                
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
                        rcvBuffer[rcvLen] = '\0';
                        printf("[%s] received\n", rcvBuffer);

//5-1
                        if(!strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요")))
                                strcpy(buffer, "안녕하세요. 만나서 반가워요");
//5-2
			
			else if(!strncmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?")))
                                strcpy(buffer, "내 이름은 ooo야.");	
			
			else if(!strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?")))
				strcpy(buffer, "나는 21살이야");

//5-3.1
			else if(!strncasecmp(rcvBuffer, "strlen ", 7))
			 sprintf(buffer, "내 문자열의 길이는 %d입니다.", strlen(rcvBuffer)-7);
//5-3.2			
			else if(!strncasecmp(rcvBuffer, "strcmp ", 7)){ 
				char *token1;
				char *str1[3];
				int i = 0;
				token1 = strtok(rcvBuffer, " ");
				while(token1 !=NULL){
					str1[i++] = token1;
					token1 = strtok(NULL, " ");
				 }
				if(i<3)
					sprintf(buffer, "문자열 비교를 위해서는 두 문자열이 필요합니다.");
				else if(!strcmp(str1[1], str1[2])) //같은 문자열이면
					sprintf(buffer, "%s와 %s는 같은 문자열입니다.", str1[1], str1[2]);
				else
					sprintf(buffer, "%s와 %s는 다른 문자열입니다.", str1[1], str1[2]);
			}else
				sprintf(buffer, "무슨 말일지 모르겠습니다.");
//6-1			
			if(!strncasecmp(rcvBuffer, "readfile ", 9)){
				FILE *fp;
				char *token2;
				char *str2[2];
				int i = 0;
				token2 = strtok(rcvBuffer, " ");
				while(token2 != NULL){
					str2[i++] = token2;
					token2 = strtok(NULL, " ");
				}
				if(i<2)
					sprintf(buffer, "readfile 기능을 이용하기 위해서는 readfile 파일명을 입력해주세요");
				fp = fopen(str2[1], "r");
				if(fp){
					char tempStr[100];//100- 미리define으로 값을 설정하는것을 권장
					memset(buffer, 0, 100);
					while(fgets(tempStr, 100, (FILE *)fp)){
						strcat(buffer,tempStr);
					}
					//while(fgets(buffer, 100, (FILE *)fp))
						//sprintf(buffer,"%s", strcat(buffer1,buffer));
				}
				fclose(fp);

			}
			if(!strncasecmp(rcvBuffer, "exec ", 5)){
                                char *token3;
                                char *str3[2];
                                int ret;
                                token3 = strtok(rcvBuffer, " ");
                                while(token3 != NULL){
                                        str3[i++] = token3;
                                        token3 = strtok(NULL, "\0");//\0->공백문자까지
                                }
				ret = system(str3[1]);
				if(!ret)//ret가 0이면
					strcpy(buffer, "command Success");
				else
					strcpy(buffer, "command Failed");
			
			}
			n = strlen(buffer);
			write(c_socket, buffer, n);
	}	
	close(s_socket);
	}
}	
