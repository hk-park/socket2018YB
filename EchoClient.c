#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 12000
#define IPADDR "127.0.0.1"


int main(){
	int c_socket, echo_socket;
	struct sockaddr_in c_addr;
	int len,n;

	char rcvBuffer[100];
	char sendBuffer[100]; //3-4 
	c_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&c_addr, 0,sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family=AF_INET; //ipv4 used
	c_addr.sin_port = htons(PORT);

	if(connect(c_socket, (struct sockaddr *)&c_addr,sizeof(c_addr))==-1)
	{
	printf("[ERR] cannot connect.\n");
	close(c_socket);
	return -1;
	}
	
	//3-4
//	strcpy(sendBuffer, "Hi I'm Client");

	
	while(1){
		fgets(sendBuffer, sizeof(sendBuffer), stdin);

			
		write(c_socket, sendBuffer, strlen(sendBuffer));
	
			if(strncasecmp("quit", sendBuffer,4)==0||strncasecmp("kill server",sendBuffer,11)==0)	
				break;		

		n = read(c_socket, rcvBuffer,sizeof(rcvBuffer));	
		rcvBuffer[n]='\0'; //이 다음부터 \0으로 초기화해버림
		printf("[%s]\nis recived.\n", rcvBuffer);
		printf("You recived %d Data len\n", n);

	}

	

	close(c_socket);
	return 0;

	}
