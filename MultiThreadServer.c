#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>

// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
#define BUFSIZ 10000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
void *do_service(void *data);
void sig_handler();
int servercount=0;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
main( )
{
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int sel = 0;
	int rcvLen;
	char rcvBuffer[100];
	char buffer[BUFSIZ] = "Server is connected";
	char *bf1,*bf2;
	int c_socket, s_socket;
	pthread_t pthread;
	int thr_id;
	int status;
	
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
		
		thr_id = pthread_create(&pthread,NULL,do_service,(void*)&c_socket);
		pthread_join(pthread,(void **) &status);
		printf("Client is connected\n");
		printf("현재 %d개의 클라이언트가 접속하였습니다.\n", ++servercount);
		
		n = strlen(buffer);
		write(c_socket, buffer, n);	
/*
		int pid = fork();
		if(pid >0){
			close(c_socket);
			continue;
		}
		else if(pid == 0){
			do_service(c_socket);
		}
*/	
	}	
//	killserver = 1;
	close(s_socket);
	//kill(getppid(),SIGKILL);
}
void *do_service(void *data)
{

	int sel = 0,n=0,re=0;
	int rcvLen;
	char rcvBuffer[100];
	char buffer[BUFSIZ] = "Server is connected";
	char *bf1,*bf2;
	int c_socket = *((int*)data);
	while(1){
		sel = 0;
		rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
		rcvBuffer[rcvLen] = '\0';
		printf("[%s] received\n", rcvBuffer);
		if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
			break;
		else if(strncasecmp(rcvBuffer,"안녕하세요.",strlen("안녕하세요."))==0)
			sprintf(buffer,"안녕하세요. 만나서 반가워요.");	
		else if(strncasecmp(rcvBuffer,"이름이 머야?",strlen("이름이 머야?"))==0)
   	sprintf(buffer,"내 이름은 안승모야");	
		else if(strncasecmp(rcvBuffer,"몇 살이야?",strlen("몇 살이야?"))==0)
	sprintf(buffer,"나는 23살이야");	
		else if(strncasecmp(rcvBuffer,"strlen",6)==0)
		{
			strtok(rcvBuffer," ");
			bf1 = strtok(NULL," ");
			sprintf(buffer,"%d",strlen(bf1));
		}
	else if(strncasecmp(rcvBuffer,"strcmp",6)==0)
	        {
			char *token;
			char *str[3];
			int i = 0;
			token = strtok(rcvBuffer," ");
			while(token != NULL)
			{
				str[i++] = token;
				token = strtok(NULL," ");
				
			}
			if(i<3)
				sprintf(buffer,"문자열 비교를 위해서는 두 문자열이 필요합니다.");
			else if(!strcmp(str[1],str[2]))
				sprintf(buffer,"%s와 %s는 같은 문자열 입니다.",str[1],str[2]);
			else
				sprintf(buffer,"%s와 %s는 다른 문자열 입니다.",str[1],str[2]);
       	 	}
       else if(strncasecmp(rcvBuffer,"readfile",8)==0)
	        {
			sel = 1;
			char fbuffer[BUFSIZ];
			char *token;
			char *str[2];
			int i = 0;
			token = strtok(rcvBuffer," ");
			while(token != NULL)
			{
				str[i++] = token;
				token = strtok(NULL," ");
				
			}
			if(i<2)
				sprintf(buffer,"문자열이 필요합니다.");
			else
			{
				FILE *fp;
				sprintf(buffer,"");
				fp = fopen(str[1],"r");
				while(fgets(fbuffer,str[1],(FILE *)fp))
					strcat(buffer,fbuffer);
				
				fclose(fp);
			}
       	 	}

       else if(strncasecmp(rcvBuffer,"exec",4)==0)
	        {
			sel = 1;
			char *token;
			char fbuffer[BUFSIZ];
			char *str[3];
			int i = 0;
			token = strtok(rcvBuffer," ");

			while(token != NULL)
			{
				str[i++] = token;
				token = strtok(NULL," ");
				
			}
			sprintf(fbuffer,"%s %s",str[1],str[2]);
			if(system(fbuffer)==0) sprintf(buffer,"command is executed");
			else sprintf(buffer,"command failed");
			
       	 	}

		else sprintf(buffer,"대답할 수 없어요");	

				
		n = strlen(buffer);		
		write(c_socket, buffer, n);
		
	}

	pthread_mutex_lock(&mutex);
	if(strncasecmp(rcvBuffer, "kill server", strlen("kill server"))==0){
		re=1;
		write(c_socket, rcvBuffer, rcvLen);
		printf("1개의 클라이언트가 접속종료되어 %d개의 클라이언트가 접속되어 있습니다.\n",--servercount);
	}
	pthread_mutex_unlock(&mutex);
	close(c_socket);
//	if(re) close(s)
	
}
/*
void sig_handler(int signo)
{
	int pid;
	int status;
	char buf[BUFSIZ];
	pid = wait(&status);
	printf("pid[%d] terminalted, status = %d\n",pid,status);
	printf("1개의 클라이언트가 접속종료되어 %d개의 클라이언트가 접속되어 있습니다.\n",--servercount);
	read(fd[0],buf,sizeof(buf));
	//printf("%s\n",buf);
	if(strncmp(buf, "kill server", strlen("kill server"))==0){
		printf("server die\n");
		close(s_socket);
		exit(0);
	}
}
*/

