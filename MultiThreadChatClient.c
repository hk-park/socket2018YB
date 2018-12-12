
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // read, write, close 함수들을 사용하기위해 필요
#include <netinet/in.h>  // internet protocol family의 주소를 저장하는데 사용
#include <sys/socket.h> // 소켓 구조를 정의하는데 사용
#include <sys/select.h> // 파일 디스크립터를 관찰하는데 사용
#include <pthread.h>  // 스레드를 사용하기위해 필요
#include <signal.h> // 신호를 위한 값 정의
#define CHATDATA 1024 // 한번에 보낼 수 있는 채팅데이터를 1024로 지정.
#define IPADDR "127.0.0.1"
#define PORT 9000

void *do_send_chat(void *);
void *do_receive_chat(void *);
pthread_t thread_1, thread_2;
char   escape[ ] = "exit"; // escape배열에 "exit" 값 저장.
char   nickname[20];  // 사용자에게 입력받을 nickname값 저장을 위한 nickname배열 선언.

int main(int argc, char *argv[ ]){
	int c_socket;
	struct sockaddr_in c_addr;
	int len;
	char chatData[CHATDATA]; // 보내고 받아올 메세지를 담을 chatData배열 선언.
	char buf[CHATDATA];  // 입력받은 메세지를 넣어둘 buf배열 선언.
 	int n;
	// 클라이언트 소켓 생성, IPv4 인터넷 프로토콜 체계 사용, 연결 지향형.
 	c_socket = socket(PF_INET, SOCK_STREAM, 0);
	memset(&c_addr, 0, sizeof(c_addr));  // 클라이언트 소켓의 주소값 설정을 위해 초기화시켜줌.
	c_addr.sin_addr.s_addr = inet_addr(IPADDR); // IPv4 인터넷 프로토콜 주소체계 사용.
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);
	printf("Input Nickname : ");
 	scanf("%s", nickname);
 // 서버에 연결.
	if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1) {
  		printf("Can not connect\n");
		return -1;
	}
	 // 스레드 생성, id는 thread_1로, 특성값은 없음, 스레드를 실행할 함수는 do_send_chat, 함수의 인자값은 c_socket.
 	pthread_create(&thread_1,NULL,do_send_chat,(void *)&c_socket);
 	pthread_create(&thread_2,NULL,do_receive_chat,(void *)&c_socket);
	 // thread_1이 끝날때까지 기다림, 리턴값은 없음.
	pthread_join(thread_1,NULL);
 	pthread_join(thread_2,NULL);

 	close(c_socket);
}

void * do_send_chat(void *arg){

	 char chatData[CHATDATA];
	 char buf[CHATDATA];
	 int n;
	 int c_socket = *((int *) arg);

 	sprintf(chatData, "[%s] connected..\n", nickname);
	write(c_socket, chatData, strlen(chatData));

 	while(1) {
		memset(buf, 0, sizeof(buf));
		// 파일디스크립터가 0이므로(표준입력) 사용자에게 입력받은값을 읽어 buf에 저장.
		if((n = read(0, buf, sizeof(buf))) > 0 ) {
	  // nickname과 buf값을 "[%s] %s"형식으로 chatData에 저장.
		sprintf(chatData, "[%s] : %s", nickname, buf);
		write(c_socket, chatData, strlen(chatData));
		// 클라이언트 소켓을 파일 디스크립터로 chatData의 값을 쓰기.
			if(!strncmp(buf, escape, strlen(escape))) {
			pthread_kill(thread_2, SIGINT);
 			break;
   			}

  		}

	}
}

void *do_receive_chat(void *arg){
	char   chatData[CHATDATA];
	int   n;
	int   c_socket = *((int *)arg);

	while(1) {
		memset(chatData, 0, sizeof(chatData));  // chatData 초기화.
		// 클라이언트 소켓에 저장되어 있는 값을 읽어 chatData에 저장.
		if((n = read(c_socket, chatData, sizeof(chatData))) > 0 ) {
		write(1, chatData, n);
		}
 	}
}
