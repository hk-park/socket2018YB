#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
#define BUFSIZE 10000

char buffer[BUFSIZE] = "Hi. I'm server\n";

main()
{
	int c_socket,s_socket;
	struct sockaddr_in s_addr,c_addr;
	int len;
	int n;
	int rcvLen;
	char rcvBuffer[BUFSIZE];
	char *ptr[BUFSIZE];
	s_socket = socket(PF_INET, SOCK_STREAM,0);

	memset(&s_addr,0,sizeof(s_addr));

	s_addr.sin_addr.s_addr =inet_addr("127.0.0.1");
	s_addr.sin_family = AF_INET;
  	s_addr.sin_port = htons(PORT);

	if(bind(s_socket,(struct sockaddr *)&s_addr,sizeof(s_addr)) == -1){
		printf("can not bind\n");
		return -1;
	}
	
	
