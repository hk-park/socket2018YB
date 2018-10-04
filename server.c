#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
//char buffer[100] = "hello world\n";
char buffer[BUFSIZ] = "Hi, I'm server\n";

int main(){
	int c_socket, s_socket;
	//s_socket: 클라이언트의 접속을 기다리는 소켓
	//c_socket: 클라이언트에게 메시지를 전달하기 위한 소켓
	struct sockaddr_in s_addr, c_aadr;
	int len, n;
	int rcvLen;
	char rcvBuffer[BUFSIZ];
	
	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);//IP주소 저장
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
	
	if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1){
		printf("[ERR] Cannot bind\n");
		return -1;
	}
	
	if(listen(s_socket, 5) == -1){
		printf("[ERR] Cannot listen\n");
		return -1;
	}
	
	while(1){
		len = sizeof(c_aadr);
		c_socket = accept(s_socket, (struct sockaddr *)&c_aadr, &len);
		printf("Client is connected\n");
			while(1){
				rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
				rcvBuffer[rcvLen]='\0';
				printf("[%s] received\n",rcvBuffer);
				if(!strncmp(rcvBuffer, "안녕하세요.", strlen("안녕하세요."))){
					strcpy(buffer, rcvBuffer);
					strcat(buffer, "만나서 반가워요.");
				}
				else if(!strncmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?"))){
					strcpy(buffer, "내이름은 xxx야.");
				}
				else if(!strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?"))){
					strcpy(buffer, "나는 xx살이야.");
				}
				if(strncasecmp(rcvBuffer, "quit", 4)==0 || strncasecmp(rcvBuffer, "kill server", 11) == 0){
					break;
				}
				n = strlen(buffer);
				write(c_socket, buffer, n);
			}
		write(c_socket, buffer, strlen(buffer));
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11)){
			break;
		}
	}
	close(s_socket);
}
