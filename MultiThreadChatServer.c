#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

void *do_chat(void *);
int pushClient(int);  // 사용자를 추가하기 위한 pushClient함수 선언.
int popClient(int);  // 사용자를 제거하기 위한 popClient함수 선언.
pthread_t thread;  // 스레드 생성.
pthread_mutex_t mutex;  // 스레드들 간에서 공유가 배제되는 객체인 mutex 선언.

typedef struct clients {
	int client_num;
	char nickname[20];
} clients;

#define MAX_CLIENT 10  // 최대 사용자를 10으로 지정.
#define CHATDATA 1024  // 최대 사용자를 10으로 지정.
#define INVALID_SOCK -1 // 할당받지 않은 소켓을 나타내기위해 INVALID_SOCK을 -1로 지정.
#define PORT 9000

clients client_list[MAX_CLIENT];  // 클라이언트의 목록을 관리할 client_list배열 선언, 크기를 10으로 함.
char   escape[ ] = "exit"; // escape배열에 "exit" 값 저장.
char   greeting[ ] = "Welcome to chatting room\n";
char   CODE200[ ] = "Sorry No More Connection\n";

int main(int argc, char *argv[ ]){
	int c_socket, s_socket;
 	struct sockaddr_in s_addr, c_addr;
 	int   len;
 	int   i, j, n;
 	int   res;

  // mutex객체를 초기화, 특성값은 없음.
	if(pthread_mutex_init(&mutex, NULL) != 0) {
	printf("Can not create mutex\n");  // mutex객체 초기화에 실패시 메세지 출력.
  	return -1;
 	}

	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 주소를 허용.
	s_addr.sin_family = AF_INET;  // IPv4 인터넷 프로토콜 주소체계 사용.
	s_addr.sin_port = htons(PORT);

  // 소켓에 주소를 부여하는 기능을 수행하는 bind 함수.
	if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	// 소켓을 연결 요청 대기상태로 만드는 기능을 수행하는 listen 함수.
 	}if(listen(s_socket, MAX_CLIENT) == -1) {
 		 printf("listen Fail\n");
  		return -1;
 	}for(i = 0; i < MAX_CLIENT; i++){ // i가 MAX_CLIENT값보다 작을경우반복.
  		client_list[i].client_num = INVALID_SOCK;
  		client_list[i].nickname == "\0";
 		}while(1) {
  			len = sizeof(c_addr); // len값을 c_addr의 크기로 설정.
			// accept함수를 통해 서버소켓에 연결요청이 있을경우 이를 수락하고 클라이언트 소켓에 반환값(생성된 소켓의 파일 디스크립터)을 저장.
  			c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
  			res = pushClient(c_socket);
  			if(res < 0) {
   				write(c_socket, CODE200, strlen(CODE200));
   				close(c_socket);
  			} else {
   				write(c_socket, greeting, strlen(greeting));
			 // 스레드 생성, id는 thread로, 특성값은 없음, 스레드를 실행할 함수는 do_chat, 함수의 인자값은 c_socket.
   				pthread_create(&thread,NULL,do_chat,(void *)&c_socket);
  			}
 		}
}
void *do_chat(void *arg){
	 int c_socket = *((int *)arg);
	 char chatData[CHATDATA], buf[CHATDATA], nick[20], tNick[20];
	 int i, n;
	 char *tok, *ntok, *front;

 	while(1) {
  		memset(chatData, 0, sizeof(chatData));
  		if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
   			if(strstr(chatData, "connected") != NULL){
					// 닉네임 저장.
    				ntok = strtok(chatData, "[]");
    				strcpy(nick, ntok);
    				for(i=0;i<MAX_CLIENT;i++){
     					pthread_mutex_lock(&mutex);
     					if(client_list[i].client_num!=INVALID_SOCK){
      						if(strcmp(client_list[i].nickname,"\0")==0){
       						strcpy(client_list[i].nickname, nick);
       						sprintf(buf,"[%s] connected! \n", client_list[i].nickname);
       						pthread_mutex_unlock(&mutex);
      						}
     					}
     					pthread_mutex_unlock(&mutex);
    				}
    				for(i=0;i<MAX_CLIENT;i++){
     				if(client_list[i].client_num!=INVALID_SOCK)
     				write(client_list[i].client_num,buf,n);
     				printf("%d %s \n", client_list[i].client_num, client_list[i].nickname);
   			 }
    			printf("\n");
			//귓속말 기능 추가.
   		} else if(strstr(chatData, "/w") != NULL) {
    			tok = strtok(chatData, " ");
    			front = tok;
    			tok = strtok(NULL, ": ");
    			tok = strtok(NULL, " "); // 받는 사람(클라이언트)의 닉네임 저장
    			strcpy(tNick, tok);
    			tok = strtok(NULL, " ");  // 보낼 문자열을 저장
    			strcpy(buf, tok);
    			while(tok != NULL) {
     				tok = strtok(NULL, " ");
     				if(tok != NULL) {
      					strcat(buf, " ");
      					strcat(buf, tok);
     				}
    			}
    			strcpy(chatData, front);
    			strcat(chatData, " : ");
    			strcat(chatData, buf);
					//귓속말 메세지 전송
    			for(i=0;i<MAX_CLIENT;i++){
     				if(strcmp(client_list[i].nickname, tNick) == 0){
      					write(client_list[i].client_num,chatData,strlen(chatData));
     				}
   			}
				// 모두에게 전송
   		}else{
   			for(i=0;i<MAX_CLIENT;i++){
     				if(client_list[i].client_num!=INVALID_SOCK)
      					write(client_list[i].client_num,chatData,n);
    			}
   		}
  	}
 }
}

int pushClient(int c_socket) {
	int i;
	for(i=0;i<MAX_CLIENT;i++){
		pthread_mutex_lock(&mutex);  // mutex객체를 이용해 다른 스레드가 접근하지 못하도록 함.
		if(client_list[i].client_num==INVALID_SOCK){
			client_list[i].client_num = c_socket;
			pthread_mutex_unlock(&mutex); // 다른 스레드가 접근할 수 있도록 함.
			return 1;
  		}
		pthread_mutex_unlock(&mutex);
 	}if(i==MAX_CLIENT)	return -1;
}

int popClient(int s){
	int i;
	close(s);
	for(i=0;i<MAX_CLIENT;i++){
		pthread_mutex_lock(&mutex);
		if(s==client_list[i].client_num){
			client_list[i].client_num = INVALID_SOCK;
			strcpy(client_list[i].nickname,"\0");
			pthread_mutex_unlock(&mutex);
			break;
		}
	pthread_mutex_unlock(&mutex);
	}
	return 0;
}
