#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#define PORT 9000
#define BUFSIZE 10000

char buffer[BUFSIZE] = "Hi. I'm server\n";
int num_client = 0; //
main()
{
	int c_socket,s_socket;
	struct sockaddr_in s_addr,c_addr;
	int len;
	int n;
	signal(SIGCHID_in s_addr); //
	int rcvLen;
	char rcvBuffer[BUFSIZE],*yjy1,*yjy2;
	char yjyBuffer[100] = "";
	char buffer2[100];
	
         
	memset(&s_addr,0,sizeof(s_addr));
        s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

  	s_socket=socket(PF_INET,SOCK_STREAM.0);
        if(bind(s_socket,(struct sockaddr *) &s_addr,sizeof(s_addr)) == -1){
		printf("Can not Bind\n");
		return -1;
	}
	if(listen(s_socket,5) == -1){
		printf("listen Fail\n");
		return -1;
	}

      
	while(1){
		len = sizeof(c_addr);
		c_socket=accept(s_socket,(struct sockaddr *)&c_addr,&len);
		if((pid = fork()) > 0){
                 	close(c_socket);
			continue;
		}else if(pid == 0){
			close(s_socket);
			do_service(conn_s);
			exit(0);
		}
		n = strlen(buffer);
		write(c_socket,buffer,n);
		close(c_socket);
	}
close(s_socket);
}

do service(int c_socket){
	while(1){
		
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr,&len);
		printf("Client is connected\n");
		while(1){

                    rcvLen = read(c_socket,rcvBuffer,sizeof(rcvBuffer));
		    rcvBuffer[rcvLen] ='\0';
	  	    buffer[0] ='\0';
		    printf("[%S] received\n",rcvBuffer);
		    if(strncasecp(rcvBuffer,"quit",4) == 0 || strncasecmp(rcvBuffer."kill server",11) == 0) {
			break;
			}
 		    else if(!strncmp(rcvBuffer."안녕하세요",strlen("안녕하세요"))){
			strcpy(buffer,"안녕하세요 만나서 반가워요.");
			n=strlen(buffer);
			write(c_socket.buffer,n);
		    }
		    else if(!strncmp(rcvBuffer."이름이 뭐야?",strlen("이름이 뭐야?"))){
			strcpy(buffer."내이름은 윤지영이야");
			n = strlen(buffer);
			write(c_socket,buffer,n);
		    ]
		    else if(!strncmp(rcvBuffer,"몇 살이야?",strlen("몇 살이야?"))){
			strcpy(buffer,"나는 22살이야");
			n = strlen(buffer);
			write(c_socket,buffer,n);
}


		   
}
} 	

do sig_handler(){
	int pid;
	int status;
	pid = wait(&status);
	printf("pid[%] is terminated status = %d\n",pid,status);
}
