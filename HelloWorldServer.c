#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
//#define PORT 9000
#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100];
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   sock, len, str_len, n;
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
 
	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		
		return -1;
	}
 
	if(listen(s_socket, 5) == -1) {
		printf("listen Fail\n");
		return -1;
	}


while(1){
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);

	while(1) {
		buffer[0]='\0';
		n = read(c_socket, buffer, sizeof(buffer));
		if(n < 0){
			printf("[ERR] Cannot read\n");
			return -1;
		}
		buffer[n] = '\0';	
		if(!strcmp(buffer,"quit\n"))
			break;
		if(!strcmp(buffer,"kill server\n")){
			close(s_socket);
			return -1 ;
		}
		printf("클라이언트로부터 받은 문자열: %s", buffer);
		if(!strcmp(buffer,"안녕하세요.\n"))
			strcpy(buffer, "안녕하세요 만나서 반가워요.\n");
		else if(!strcmp(buffer,"이름이 머야?\n"))
			strcpy(buffer, "내 이름은 Siri야.\n");
		else if(!strcmp(buffer,"몇 살이야?\n"))
			strcpy(buffer, "나는 XX살이야.\n");
		else if(!strncasecmp(buffer, "strlen", 6)){
			
			sprintf(buffer, "%d", );
			strcpy(buffer, strlen(buffer)-7);
		}
		else
			strcpy(buffer, "만일 내게 물어보면 나는 서버\n");
		write(c_socket, buffer, strlen(buffer));
	}	

	close(c_socket);
}
	close(s_socket);
}





