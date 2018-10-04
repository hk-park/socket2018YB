#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 10000
#define IPADDR "127.0.0.1"

int main(){
	int c_socket;
	struct sockaddr_in c_addr;
	int n;
	char rcvBuffer[100];
	char buffer[100];
	c_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);

	if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1){
		printf("[ERR] Cannot connect\n");
		close(c_socket);
		return -1;
	}
	printf("[INFO] Successfully connected to server\n");
	while(1){
	buffer[0] = '\0';
	printf("[SEND] : ");
	fgets(buffer, sizeof(buffer), stdin);
	n = write(c_socket, buffer, strlen(buffer));
	
	if(!strcmp(buffer,"quit\n"))
		break;
	if(!strcmp(buffer,"kill server\n"))
		break;

	if(n < 0){
		printf("[ERR] Cannot write\n");
		return -1;
	}

	n = read(c_socket, rcvBuffer, sizeof(rcvBuffer));

	if(n < 0){
		printf("[ERR] Cannot read\n");
		return -1;
	}
	rcvBuffer[n] = '\0';

	printf("[INFO] 서버로부터 받은 문자열 : %s", rcvBuffer);
	printf("[INFO] 서버로부터 받은 문자열의 길이: %d\n", strlen(rcvBuffer));

	}
	close(c_socket);
	return 0;

}
