#include <stdio.h>

#include <stdlib.h>

#include <netinet/in.h>

#include <sys/socket.h>

#include <signal.h>

#include <sys/wait.h>

#include <string.h>

#define PORT 10000

#define BUFSIZE 10000

char Hbuffer[BUFSIZE] = "안녕하세요. 만나서 반가워요\n";

char Nbuffer[BUFSIZE] = "내 이름은 유요섭이야.\n";

char Abuffer[BUFSIZE] = "나는 21살이야.\n";

char nothing[BUFSIZE] = "아무것도 아님\n";

void sig_handler();

int clients=0;

int fd[2];

int s_socket;

main( )

{

	FILE *fp;

	int   c_socket;

	struct sockaddr_in s_addr, c_addr;

	int   len;

	int   n, i, x, pid, conn_s;

	int rcvLen;

	char rcvBuffer[BUFSIZE], *ptr,  *ptr1, *ptr2, buffer[BUFSIZE], fbuffer[BUFSIZE];

  

 	s_socket = socket(PF_INET, SOCK_STREAM, 0);

	signal(SIGCHLD, sig_handler);

	memset(&s_addr, 0, sizeof(s_addr));

	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	s_addr.sin_family = AF_INET;

	s_addr.sin_port = htons(PORT);

  

  if(pipe(fd) < 0){

    printf("pipe error\n");

    exit(1);

  }

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

		clients= clients + 1;

		printf("Client is connected\n");

		printf("현재 %d개의 클라이언트가 접속하였습니다.\n", clients);

		pid = fork();

		if(pid > 0){

			close(c_socket);

			continue;

		}else if(pid == 0){

			close(s_socket);

			do_service(c_socket);

			exit(0);

		}

    close(c_socket);

	}	

	close(s_socket);

}

do_service(int c_socket){

	FILE *fp;	

	int   len;

	int   s_socket, n, i, x;

	int rcvLen;

	char rcvBuffer[BUFSIZE], *ptr,  *ptr1, *ptr2, buffer[BUFSIZE], fbuffer[BUFSIZE];

	while(1){

		rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));

		rcvBuffer[rcvLen] = '\0';

		printf("[%s] received\n", rcvBuffer);

		buffer[0] = '\0';

		if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)

			break;

		if(!strncasecmp(rcvBuffer, "안녕하세요.", 6)){

			n = strlen(Hbuffer);

			write(c_socket, Hbuffer, n);

		}

		else if(!strncasecmp(rcvBuffer, "이름이 뭐야?", 7)){

			n = strlen(Nbuffer);

			write(c_socket, Nbuffer, n);

		}

		else if(!strncasecmp(rcvBuffer, "몇 살이야?", 6)){

			n = strlen(Abuffer);

			write(c_socket, Abuffer, n);

		}

		else if(!strncasecmp(rcvBuffer, "strlen", 6)){

			x = strlen(rcvBuffer)-7;			

			sprintf(buffer,"길이 : %d",x);

			write(c_socket, buffer, strlen(buffer));

		}

		else if(!strncasecmp(rcvBuffer, "strcmp", 6)){

			ptr = strtok(rcvBuffer, " ");

			ptr = strtok(NULL, " ");

			ptr1 = ptr;

			ptr = strtok(NULL, " ");

			ptr2 = ptr;

			x = strcmp(ptr1, ptr2);

			sprintf(buffer,"결과 : %d",x);

			write(c_socket, buffer, strlen(buffer));

		}

		else if(!strncasecmp(rcvBuffer, "readfile", 8)){

			ptr = strtok(rcvBuffer, " ");

			ptr = strtok(NULL, " ");

			ptr1 = ptr;

			fp = fopen(ptr1, "r");

			if(fp){

				while(fgets(fbuffer, BUFSIZE, (FILE *)fp))

						strcat(buffer, fbuffer);

			}

			else{

				strcpy(buffer, "존재하지 않는 파일");

			}

			fclose(fp);	

			write(c_socket, buffer, strlen(buffer));			

		}

		else if(!strncasecmp(rcvBuffer, "exec", 4)){

			ptr = strtok(rcvBuffer, " ");

			ptr = strtok(NULL, "NULL");

			ptr1 = ptr;			

			int ret = system(ptr1);

			if(!ret)

				sprintf(buffer, "<%s> command Success!!\n", ptr);

			else

				sprintf(buffer, "<%s> command Failed!!\n", ptr);

			write(c_socket, buffer, strlen(buffer));

		}

		else{

			n = strlen(nothing);

			write(c_socket, nothing, n);

		}

	}

	close(c_socket);

  write(fd[1], rcvBuffer, rcvLen);

}

void sig_handler(){

	int pid;

	int status;

	char rcvBuffer[100];

	pid = wait(&status);

	clients = clients - 1;

	printf("1개의 클라이언트가 접속종료되어  %d개의 클라이언트가 접속하였습니다.\n", clients);

	printf("pid %d is terminated status = %d\n", pid, status);
	read(fd[0], rcvBuffer, 100);

	if(strncasecmp(rcvBuffer, "kill server", 11) == 0){

    printf("kill server\n");

    close(s_socket);

    exit(0);

  }

}
