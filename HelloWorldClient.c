#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#define PORT 9000
#define BUFSIZ 10000
#define IPADDR "127.0.0.1"
char ss[100];
main()
{
	int c_socket,s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len, n;
	char rcvBuffer[BUFSIZ];
	c_socket = socket(PF_INET, SOCK_STREAM,0);
	
	memset(&c_addr,0,sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);
	if(connect(c_socket,(struct sockaddr *)&c_addr, sizeof(c_addr))==-1)
	{
		printf("faile");	
		close(c_socket);
		return -1;
	}
	if((n = read(c_socket,rcvBuffer,sizeof(rcvBuffer)))<0)return -1;
	rcvBuffer[n] = '\0';
	printf("%s\n",rcvBuffer);

	while(1){
		fgets(ss,sizeof(ss),stdin);
		n = strlen(ss);
		ss[n-1] = '\0';
                if(strcmp(ss,"quit")==0 || strcmp(ss,"kill server")==0)break;
		write(c_socket,ss,strlen(ss));
		n = n=read(c_socket,rcvBuffer,sizeof(rcvBuffer));
		if(n<0){
			printf("error\n");
			return -1;
		}
                rcvBuffer[n] = '\0';
                printf("%s\n",rcvBuffer,n);

	}
	close(c_socket);
}
