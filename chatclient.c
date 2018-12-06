#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>

#define PORT 9000
#define BUFSIZ 10000
#define IPADDR "127.0.0.1"
int readchat(int c_socket);
void *sendchat(void *c_socket);
int idcheck(int c_socket);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
main()
{
	int c_socket,s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len, n;
	char rcvBuffer[BUFSIZ], ss[100];

	pthread_t pthread;
	int thr_id;

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
	if(idcheck(c_socket) == 0) return -1;
	thr_id = pthread_create(&pthread,NULL,sendchat,(void*)&c_socket);
	pthread_join(pthread,(void **) &status);
	readchat(c_socket);

	close(c_socket);
}

int idcheck(int c_socket){
	char sendBuffer[BUFSIZ], rcvBuffer[BUFSIZ];
	while(1){
		printf("사용할 아이디를 입력해주세요\n");
		memset(&sendBuffer,0,sizeof(sendBuffer));
		fgets(sendBuffer,sizeof(sendBuffer),stdin);
		n = strlen(sendBuffer);
		sendBuffer[n-1] = '\0';
		write(c_socket,sendBuffer,strlen(sendBuffer));
		memset(&rcvBuffer,0,sizeof(rcvBuffer));
		n = read(c_socket,rcvBuffer,sizeof(rcvBuffer));
		if(n<0){
			printf("error\n");
			return 0;
		}
		rcvBuffer[n] = '\0';
		printf("%s\n",rcvBuffer,n);
		if(strcmp(rcvBuffer,"access") == 0) {
			printf("사용가능한 아이디 입니다.\n");
			return 1;
		}
		else {
			printf("중복되는 아이디 입니다. 다시 ");
		}
	}
}
void *sendchat(void *c_socket){
	char sendBuffer[BUFSIZ];
	while(1)
	{
		memset(&sendBuffer,0,sizeof(sendBuffer));
		fgets(sendBuffer,sizeof(sendBuffer),stdin);
		n = strlen(sendBuffer);
		sendBuffer[n-1] = '\0';
		write(c_socket,ss,strlen(ss));
	}
}

int readchat(int c_socket){
	char rcvBuffer[BUFSIZ];
	while(1){
		memset(&rcvBuffer,0,sizeof(rcvBuffer));
		n = read(c_socket,rcvBuffer,sizeof(rcvBuffer));
		if(n<0){
			printf("error\n");
			return 0;
		}
		rcvBuffer[n] = '\0';		
		if(strcmp(sendBuffer,"quit")==0 || strcmp(rcvBuffer,"kill server")==0)
			return 1;
		printf("%s\n",rcvBuffer);
	}
}

