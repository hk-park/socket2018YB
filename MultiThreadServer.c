#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "Hi\n";

	int s_socket,c_socket;
        struct sockaddr_in s_addr, c_addr;
        int   len;
        int   n, tn;

        pthread_t pthread;
        int thr_id;
        int rcvLen;
        char rcvBuffer[100];
        char *t1,*t2,*token;
        char i[100];
        char str[100];
 
main( )
{	int s_socket,c_socket;
	struct sockaddr_in s_addr, c_addr;

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

		thr_id = pthread_create(&pthread,NULL,do_service,(void *)&c_socket);
		}
	}	
	void *do_service(void *data)
	{
        	char rcvBuffer[BUFSIZ];
	        int   n, tn;
		int c_socket=*((int*)data);

		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
			if(strncasecmp(rcvBuffer, "quit", 4) == 0
			 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
		if(strncasecmp(rcvBuffer,"안녕하세요",strlen("안녕하세요"))==0){
			strcpy(i,"안녕하세요. 만나서 반가워요.\n");
			write(c_socket,i,strlen(i));
		}
		else if(strncasecmp(rcvBuffer,"이름이 뭐야?",
					strlen("이름이 뭐야?"))==0){
                        strcpy(i,"내 이름은 서버야.\n");
                        write(c_socket,i,strlen(i));
                }       
		else if(strncasecmp(rcvBuffer,"몇 살이야",
					strlen("몇 살이야"))==0){
                        strcpy(i,"나는 20살이야.\n");
                        write(c_socket,i,strlen(i));
                }
		else if(strncasecmp(rcvBuffer,"strlen",strlen("strlen"))==0){
			sprintf(i,"%d\n",strlen(rcvBuffer)-7);
			write(c_socket,i,strlen(i));
		}       
		else if(strncasecmp(rcvBuffer,"strcmp",strlen("strcmp"))==0){
		//	strtok(rcvBuffer," ");
		//	t1 = strtok(NULL, " ");
		//	t2 = strtok(NULL, " ");
		//	tn = strcmp(t1,t2);
		//	sprintf(i,"%d\n",tn);
		//	write(c_socket,i,strlen(i));
			*token=0;
			char *str[3];
			int num = 0;
			token = strtok(rcvBuffer," ");
			while(token !=NULL){
				str[num++] = token;
				token = strtok(NULL," ");
			write(c_socket,i,strlen(i));
			}
		if(num<3)
			sprintf(i,"문자열 비교를 위해 두 문자열이 필요합니다.");
		 else if(!strcmp(str[1],str[2]))//같은 문자열이면,
			sprintf(i,"%s와 %s는 같은 문자열입니다.",str[1],str[2]);
		 else  
			sprintf(i,"%s와 %s는 다른 문자열입니다.",str[1],str[2]);
		}


		else if(strncasecmp(rcvBuffer, "readfile ", 9) == 0){
				char *token;  
 				FILE *fp;  
				char buffer[200];  
				strtok(rcvBuffer, " ");  
				token = strtok(NULL, " ");  
 				fp = fopen(token, "r");  
				if(fp){  
					while(fgets(rcvBuffer, 100, (FILE *)fp))  
					write(c_socket, i, strlen(i));  
				}  
				
			}  
			else if(strncasecmp(rcvBuffer, "exec ", 5) == 0){ 
				char *token; 
 				strtok(rcvBuffer, " ");  
				token = strtok(NULL, "\0");  
				int ret = system(token);  
				char *exe="command is executed";  
				char *fail="command failed";  
 				if(!ret){ 
 					write(c_socket, exe, strlen(exe));
					printf("\n");
 
				}else  {
					write(c_socket, fail, strlen(fail));
					printf("\n");
				}  
		}  
//		else if(!strncasecmp(rcvBuffer, "readfile", 9)){
//			int ni=0;
//			token = strtok(rcvBuffer, " ");
//			while(token != NULL){
//				str[ni++] = token;
//				token = strtok(NULL, " ");
//			}
			//str[0] = readfile
                        //str[1] = filename
//			if(ni<2)
//				sprintf(buffer,"readfile 기능을 사용하기 위해서는 readfile <파일명> 형태로 입력하시오.");
//			FILE *fp = fopen(str[1], "r");
//			if(fp){
//				char tempStr[100];
//				memset(buffer, 0,100);
//				while(fgets(tempStr,100,(FILE *)fp)){
//					strcat(buffer, tempStr);
//				}
//				fclose(fp);	
//			}else{
//				sprintf(buffer,"파일이 없습니다");
//			     }
//			}
		n = strlen(buffer);
		write(c_socket, buffer, n);
		}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
		break;
	}	
	close(s_socket);
}



