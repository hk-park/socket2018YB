#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 8000
 
char buffer1[100] = "Hi, nice to meet you.\n";
char buffer2[100] = "My name is ChBot\n";
char buffer3[100] = "I'm 24\n";

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
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;

			if(strncasecmp(rcvBuffer, "hi", 2) == 0 || strncasecmp(rcvBuffer, "hello", 5) == 0) {
				n = strlen(buffer1);
				write(c_socket, buffer1, n);
			}
			if(strncasecmp(rcvBuffer, "what is your name?", 18) == 0 || strncasecmp(rcvBuffer, "what's your name?", 17) == 0) {
				n = strlen(buffer2);
				write(c_socket, buffer2, n);
			}
			if(strncasecmp(rcvBuffer, "how old are you?", 15) == 0) {
				n = strlen(buffer3);
				write(c_socket, buffer3, n);
			}
		}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}
