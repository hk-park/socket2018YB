#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>

#define PORT 10000
#define MAX 10000

char buffer[MAX];
void *do_service(void *data);
int   c_socket, s_socket;
struct sockaddr_in s_addr, c_addr;
int   len;
int   n;
int rcvLen;
char rcvBuffer[MAX];
char *token;
int i=0;
pthread_t pthread;
int thr_id;

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
		thr_id = pthread_create(&pthread, NULL, do_service, (void *)&c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}
	close(s_socket);
}
void *do_service(void *data){
        int c_socket=*((int *)data);
                while(1){
                        rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
                        rcvBuffer[rcvLen] = '\0';
                        printf("[%s] received\n", rcvBuffer);
                        if(strncasecmp(rcvBuffer, "quit", 4) == 0)
                                break;
                        else if(strncasecmp(rcvBuffer, "kill server", 11) == 0){
                                break;
                        }
                        else if(!strncmp(rcvBuffer,"안녕하세요.",strlen("안녕하세요."))){
                                strcpy(buffer, "안녕하세요, 만나서 반가워요.\n");
                        }
                        else if(!strncmp(rcvBuffer,"이름이 뭐야?",strlen("이름이 뭐야?")))
                                strcpy(buffer, "내 이름은 한미수야\n.");

                        else if(!strncmp(rcvBuffer,"몇살이야?",strlen("몇살이야?")))
                                strcpy(buffer, "나는 21살이야.\n");

                        else if(!strncasecmp(rcvBuffer, "strlen ", 7)){ 
                                sprintf(buffer, "내 문자열의 길이는 %d입니다.", strlen(rcvBuffer)-7);
                        }
                        else if(!strncasecmp(rcvBuffer, "strcmp ", 7)){
                                char *str[3]; 
                                i=0;
                                token=strtok(rcvBuffer, " ");
                                while(token != NULL){
                                        str[i++]=token;
                                        token=strtok(NULL, " ");
                                }
                                if(i<3)
                                        sprintf(buffer, "문자열 비교를 위해서는 두 문자열이 필요합니다.");
                                else if(!strcmp(str[1],str[2]))
                                        sprintf(buffer, "%s와 %s는 같은 문자열입니다.", str[1], str[2]);
                                else
                                        sprintf(buffer, "%s와 %s는 다른 문자열입니다.", str[1], str[2]);
                        }
                         else if(!strncasecmp(rcvBuffer, "readfile ", 9)){
                                FILE *fp;
                                char *rstr[2];
                                i=0;
                                token=strtok(rcvBuffer, " ");
                                while(token != NULL){
                                        rstr[i++]=token;
                                        token=strtok(NULL, " ");
                                }
                                if(i<2)
                                        sprintf(buffer, "readfile 기능을 사용하기 위해서는 readfile <파>일명> 형태로 입력하시오.");
                                fp=fopen(rstr[1],"r");
                                if(fp){
                                        memset(buffer, 0, MAX);
                                        while(fgets(buffer, MAX, (FILE *)fp))
                                                printf("%s", buffer);
                                }
                                else
                                        sprintf(buffer, "파일이 존재하지 않습니다.");
                                fclose(fp);
                        }
                        else if(!strncasecmp(rcvBuffer, "exec", 4)){
                                char *estr;
                                i=0;
                                token=strtok(rcvBuffer, " ");
                                estr=strtok(NULL,"\0");
                                int ret=system(estr);
                                if(!ret)
                                        sprintf(buffer, "[%s] command Success!!\n");
                                else
                                        sprintf(buffer, "[%s] command Failed!!\n");
                        }
                        else
                                strcpy(buffer, "무슨 말인지 모르겠습니다.");
                        n = strlen(buffer);
                        write(c_socket, buffer, n);
        }
        close(c_socket);
}
