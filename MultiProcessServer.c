#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#define PORT 8000
#define BUFSIZE 10000

char buffer[BUFSIZE]; 

void do_service(int c_socket);
void sig_handler();

int count_Cli = 0;	// stackoverflow 사이트
			// 전역변수로 지정하지 말고 sig_handler에 인자로 넘겨서 사용하는 방법도 있음
main( ) {
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int pid = 0;
        int   len;
        int   n;
        int rcvLen;
	signal(SIGCHLD, sig_handler);
        char rcvBuffer[BUFSIZE];
        s_socket = socket(PF_INET, SOCK_STREAM, 0);
        FILE *fp;
        char buffer2[BUFSIZE];

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
		printf("Client is connected\n");

		if((pid = fork()) > 0) {
			count_Cli++;
			printf("Connect Client is : %d\n", count_Cli);
			close(c_socket);
			continue;
		}
		else if(pid == 0) {
			close(s_socket);
			do_service(c_socket);
			exit(0);
		}
	}
}

void do_service(int c_socket) {
	int s_socket;
	struct sockaddr_in s_addr, c_addr;
        int len;
        int n;
        int rcvLen;
        char rcvBuffer[BUFSIZE];
        s_socket = socket(PF_INET, SOCK_STREAM, 0);
        FILE *fp;
        char buffer2[BUFSIZE];
	len = sizeof(c_addr);

	while(1){
		char *token;
                char *str[2];
                int i = 0;
		rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
		rcvBuffer[rcvLen] = '\0';
		printf("[%s] received\n", rcvBuffer);

		if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0) 			break;
		else if(!strncasecmp(rcvBuffer, "hi", 2)) {
			strcpy(buffer, "Hi. nice to meet you.");
		}
		else if(!strncasecmp(rcvBuffer, "what is your name?", 18)) {
			strcpy(buffer, "my name is Chbot.");
		}
		else if(!strncasecmp(rcvBuffer, "how old are you?", 15)) {
			strcpy(buffer,"I'm 24");
		}
		else if(!strncasecmp(rcvBuffer, "strlen ", 7)) {
			sprintf(buffer, "String lengh : %d", strlen(rcvBuffer) - 7);
		}
		else if(!strncasecmp(rcvBuffer, "strcmp ", 7)) {
			char *token;
			char *str[3];
			int i = 0;
			token = strtok(rcvBuffer, " ");
				while(token != NULL) {
				str[i++] = token;
				token = strtok(NULL, " ");
			}
			if(i < 3)
				sprintf(buffer, "Not enough String");
			else if(!strcmp(str[1], str[2])) {
				sprintf(buffer, "String[1] : %s\tString[2] : %s\tEqual!!", str[1], str[2]);
			}
			else {		
				sprintf(buffer, "String[1] : %s\tString[2] : %s\tNOT Equal!!", str[1], str[2]);
			}
		}
		else if(!strncasecmp(rcvBuffer, "open", 4)) {
			fp = fopen("test.txt", "r");
			if(fp) {
                       		while(fgets(buffer, BUFSIZE, (FILE *)fp)) {
                               		n = strlen(buffer);
					write(c_socket, buffer, n);
				}
                	}
                       	fclose(fp);
		}
                else if(!strncasecmp(rcvBuffer, "exec ", 5)) {
                	char *command;
                        token = strtok(rcvBuffer, " ");
			command = strtok(NULL, "\n");
			printf("command : %s\n", command);

                        int ret = system(command);
                        if(!ret)
                                sprintf(buffer, "[%s] Command Success!!\n", command);
                        else
                                sprintf(buffer, "[%s] Command Failed!!!\n", command);
                }
                else if (!strncasecmp(rcvBuffer, "readfile ", 9)) {
                        i = 0;
                        token = strtok(rcvBuffer, " ");
                        while(token != NULL) {
                                str[i++] = token;
                                token = strtok(NULL, " ");
                        }
                        if(i < 2)
                                sprintf(buffer, "[ERR] cannot read. readfile <filename> ");

                        FILE *fp = fopen(str[1], "r");
                        if(fp) {
                                char tempStr[BUFSIZE];
                                memset(buffer, 0, BUFSIZE);
                                while(fgets(tempStr, BUFSIZE, (FILE*)fp)) {
                                        strcat(buffer, tempStr);
                                }
                                fclose(fp);
                        }
                        else {
                                sprintf(buffer, "not Exist FILE");
                        }
                }
                else {
                        strcpy(buffer, "Sorry. I don't Understand..");
		}
		n = strlen(buffer);
		write(c_socket, buffer, n);
	}
	close(c_socket);
}
void sig_handler() {
	int pid;
	int status;
	pid = wait(&status);	// 자식프로세스의 종료까지 기다렷다 정상이면 0 비정상이면 에러코드 출력
	count_Cli--;
	printf("pid[%d] is terminated. status = %d\n", pid, status);
	printf("Client is quit. Connect Client is : %d\n", count_Cli);
}
