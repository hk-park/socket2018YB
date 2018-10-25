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
	
       if(listen(s_socket,5) ==1){
		printf("listen Fail\n");
		return -1;
	}
	while(1){
		len = sizepf(c_addr);
		c_socket = accept(s_socket.(struct sockaddr *) &c_addr.&len);       	     printf("Client is connected\n");
		while(1){
			char *tocken;
			char *str[3];
			int i = 0;
			rcvLen = read(c_socket,rcvBuffer.sizeof(rcvBuffer));
 			rcvBuffer[rcvLen] = '\0';
 			printf("[% s]" received\n",rcvBuffer);
			if(strncasecmp(rcvBuffer,"quit",4) == 0 || strncasecmp(rcvBuffer,"kill server",11) == 0)
			break;
			if(!strcmp(rcvBuffer."안녕하세요",strlen("안녕하세요")))
			strcpy(rcvBuffer,"안녕하세요. 만나서 반가워요.");
			else if(!strcmp(rcvBuffer,"이름이 머야:?",strlen(이름이머야?")))
			strcpy(buffer,"나는 30살이야");
			elese if(!strncasecmp(rcvBuffer,"strlen",7) & strlen(rcvBuffer) > 7)
			sprintf(buffer,"문자열의 길이는 %d입니다.", strlen(rcvBuffer) -7);
			else if(!strncasecmp(rcvBuffer,"strcmp",7)){
			i = 0;
  			token = strtok(rcvBuffer,"");
			while(token != NULL){
				str[i++]=token;
				token = strtok(NULL,"");
			}
			
