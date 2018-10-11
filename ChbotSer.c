#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#define PORT 8000

char buffer[100]; 

main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char rcvBuffer[100];
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
		printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);

			if(!strncasecmp(rcvBuffer, "open", 4)) {
				FILE *fp;
				char buffer2[100];

				fp = fopen("test.txt", "r");
				if(fp) {
					while(fgets(buffer, 100, (FILE *)fp)) {
						n = strlen(buffer);
						write(c_socket, buffer, n);
					}
				}
				fclose(fp);
			}
			else if(!strncasecmp(rcvBuffer, "exec", 4)) {
				char *token;
				char *str[2];
				int i = 0;
				int ret;
				token = strtok(rcvBuffer, " ");

				while(token != NULL) {
					str[i++] = token;
					token = strtok(NULL, " ");
				}
				if(!strcmp(str[1], "mkdir"))
					ret = system("mkdir testdir");
				else if(!strcmp(str[1], "ls"))
					ret = system("ls > list.txt");
				if(!ret)
					strcpy(buffer, "Command Success!!\n");
				else
					strcpy(buffer, "Command Failed!!!\n");
			}
			else if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
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
			else {
				strcpy(buffer, "Sorry. I don't Understand..");
			}
			n = strlen(buffer);
			write(c_socket, buffer, n);
		}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}
