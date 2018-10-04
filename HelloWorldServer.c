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
	int   c_socket, s_socket, c_strlen;
	struct sockaddr_in s_addr, c_addr;
	int   sock, len, str_len, n, cnt;
	char *ptr, *ptr1, *ptr2;
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
		printf("[ERR] listen Fail\n");
		return -1;
	}


while(1){
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		printf("[INFO] Client is connected\n");
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
		printf("[INFO] 클라이언트로부터 받은 문자열: %s", buffer);
		if(!strncasecmp(buffer,"안녕하세요", strlen("안녕하세요")))//5-1, 5-2 리눅스 3byte 윈도우 2byte인 한글처리 호환성을 위해 strlen사용, 안녕하세요로 시작하면 작동
			strcpy(buffer, "안녕하세요 만나서 반가워요.\n");
		else if(!strncasecmp(buffer,"이름이 머야", strlen("이름이 머야")))
			strcpy(buffer, "내 이름은 Siri야.\n");
		else if(!strncasecmp(buffer,"몇 살이야", strlen("몇 살이야")))
			strcpy(buffer, "네가 상상하는 그 이상입니다.\n");
		else if(!strncasecmp(buffer, "strlen", 6)){ //5-3 strlen 기능추가
			c_strlen = strlen(buffer)-8;	
			sprintf(buffer, "strlen 결과 : %d\n", c_strlen);
		}
		else if(!strncasecmp(buffer, "strcmp", 6)){ //5-3 strcmp 기능추가
			cnt = 0;
			ptr = strchr(buffer, ' ');//NULL을 만날때까지 토큰(공백)의 갯수를 세는 코드 2개 이하이면 strcmp 코드 건너뜀
			while (ptr != NULL){
       		cnt++;
        		ptr = strchr(ptr + 1, ' ');
   			 }
			if(2>cnt){
				strcpy(buffer, "strcmp 명령 인자를 제대로 입력하거라.\n");
				write(c_socket, buffer, strlen(buffer));
				continue;
			}
			ptr = strtok(buffer, " ");//두번째 토큰부터 포인터 변수에 저장후 비교
			ptr = strtok(NULL, " ");
			ptr1 = ptr;
			ptr = strtok(NULL, "\n");
			ptr2 = ptr;
			sprintf(buffer, "strcmp 결과 : %d\n", strcmp(ptr1, ptr2));
		}
		else
			strcpy(buffer, "뭐라는지 잘 모르겠어요.\n");//기본응답
		write(c_socket, buffer, strlen(buffer));
	}	

	close(c_socket);
}
	close(s_socket);
}





