#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정
//#define PORT 10000
#define PORT 9000
#define BUFSIZE 10000
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[BUFSIZE];
void sig_handler();
int clientCnt=0;
main( )
{
	int   c_socket, s_socket, c_strlen, pid;
	struct sockaddr_in s_addr, c_addr;
	int   sock, len;
	signal(SIGCHLD, sig_handler);
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

while(1) {
	len = sizeof(c_addr);
	c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
	printf("[INFO] Client is connected\n");
	printf("[INFO] 현재 %d개의 클라이언트가 접속하였습니다.\n", ++clientCnt);
	if((pid = fork( )) > 0) {// 부모 프로세스
	// 다른 클라이언트의 요청 접수
	close(c_socket);
	continue;

	} else if(pid == 0) {// 자식 프로세스:
	// 클라이언트의 요청 처리
	close(s_socket);
	do_service(c_socket);
	break;
	}
}
}

do_service(int c_socket){
int   c_strlen, n, cnt;
char *ptr, *ptr1, *ptr2;
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
			ptr = strchr(buffer, ' ');//NULL을 만날때까지 토큰(공백)의 갯수를 세는 코드 2개  미만이면 strcmp 코드 건너뜀
			while (ptr != NULL){
       		cnt++;
        		ptr = strchr(ptr + 1, ' ');
   			 }
			if(2>cnt){
				strcpy(buffer, "[ strcmp 문자열1 문자열 2 ] 형식으로 입력해주세요.\n");
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
		else if(!strncasecmp(buffer, "readfile", 8)){ //6-1 챗봇 기능추가
				char *token;
				char *str[2];
				char tempbuffer[BUFSIZE];
				int i = 0;
				FILE *fp=NULL;
				token = strtok(buffer, " ");
				while(token != NULL){
					str[i++] = token;
					token = strtok(NULL, " ");
				}
				if(i < 2){
					sprintf(buffer, "[ readfile 문자열1 ] 형식으로 입력해주세요.\n");
					write(c_socket, buffer, strlen(buffer));
					continue;
				}
				str[1][strlen(str[1]) - 1] = '\0'; //토큰의 개행문자 제거
				fp = fopen( str[1], "r");
				//buffer[0]='\0';
				if(fp){
						memset(buffer, 0, BUFSIZE); //버퍼 초기화
						while(fgets(tempbuffer, BUFSIZE, (FILE *)fp))
							strcat(buffer,tempbuffer);
						fclose(fp);//파일닫기
				}
				else
					strcpy(buffer, "파일을 찾을 수 없습니다.\n");

		}
		else if(!strncasecmp(buffer, "exec", 4)){ //6-2 system() 함수를 활용한 명령어 실행 실습
			int ret;
			cnt = 0;
			ptr = strchr(buffer, ' ');//NULL을 만날때까지 토큰(공백)의 갯수를 세는 코드 1개 미만이면 exec 코드 건너뜀
			while (ptr != NULL){
       		cnt++;
        		ptr = strchr(ptr + 1, ' ');
   			 }
			if(1>cnt){
				strcpy(buffer, "[ exec \"bash명령어\" ] 형식으로 입력해주세요.\n");
				write(c_socket, buffer, strlen(buffer));
				continue;
			}
			ptr = strtok(buffer, " ");//두번째 토큰부터 포인터 변수에 저장후 비교
			ptr = strtok(NULL, "\n");
			ptr1 = ptr;
				ret = system(ptr1);
				if(!ret)
					sprintf(buffer, "%s 명령어 실행에 성공하였습니다.\n", ptr1);
				else
					sprintf(buffer, "%s 명령어 실행에 실패하였습니다.\n", ptr1);
		}
		else
			strcpy(buffer, "뭐라는지 잘 모르겠어요.\n");//기본응답
		write(c_socket, buffer, strlen(buffer));
	}
	close(c_socket);
}
void sig_handler(){
	int pid;
	int status;
	pid = wait(&status);
	printf("[info] pid[%d] is terminated. status = %d\n", pid, status);
	printf("[info] 1개의 클라이언트가 접속종료되어 %d개의 클라이언트가 접속되어 있습니다.\n", --clientCnt);
}

