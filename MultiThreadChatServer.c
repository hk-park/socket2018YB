#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

void *do_chat(void *); //채팅 메세지를 보내는 함수
int pushClient(int); //새로운 클라이언트가 접속했을 때 클라이언트 정보 추가
int popClient(int); //클라이언트가 종료했을 때 클라이언트 정보 삭제

pthread_t thread;
pthread_mutex_t mutex;

#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000

struct list {
	int c_socket;
	char nickname[30];
	uint8_t room;
} list[MAX_CLIENT];

int    list_c[MAX_CLIENT]; //접속한 클라이언트를 관리하는 배열
char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";

int main(int argc, char *argv[ ]) {
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int    len;
    int    i, j, n;
    int    res;

    if(pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Can not create mutex\n");
        return -1;
    }

    s_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);

    if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
        printf("Can not Bind\n");
        return -1;
    }
    if(listen(s_socket, MAX_CLIENT) == -1) {
        printf("listen Fail\n");
        return -1;
    }

    for(i = 0; i < MAX_CLIENT; i++)
        list_c[i] = INVALID_SOCK;

    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
        res = pushClient(c_socket); //접속한 클라이언트를 list_c에 추가

        if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
        }
    }
}

void *do_chat(void *arg) {
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
    char sendData[CHATDATA];

    int i;
    int n;
    int client_index = -1;

    while(1) {
        memset(chatData, 0, sizeof(chatData));

        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
		client_index = searchClient(c_socket);
	  	memset(sendData, 0, sizeof(sendData));
	        sprintf(sendData,"[%s] %s",list[client_index].nickname,chatData);
	}

        else if(strncmp(chatData,"/n ",strlen("/n "))==0){
		strtok(chatData," ");
    		char* token = strtok(NULL," ");
        	strcpy(list[client_index].nickname,token);
        }

        else if(strncmp(chatData,"/r ",strlen("/r "))==0){
 	       strtok(chatData," ");
    		char* token = strtok(NULL," ");
   		list[client_index].room=token[0]-48;
        	sprintf(sendData,"[SYSTEM]Accept %d th room.\n",list[client_index].room);
        	write(list[client_index].c_socket,sendData,strlen(sendData));
        }

        else if(strncmp(chatData,"/list",strlen("/list"))==0){
        	memset(sendData, 0, sizeof(sendData));
        	for(i=0; i<MAX_CLIENT; i++){
          		if(list[i].c_socket!=INVALID_SOCK && list[client_index].room==list[i].room){
               			sprintf(sendData,"%s\n",list[i].nickname);
              			write(list[client_index].c_socket,sendData,strlen(sendData));
            		}
          	}
        }

        else if(strncmp(chatData,"/w ",strlen("/w "))==0){
        	strtok(chatData," ");
        	char* nickname = strtok(NULL," ");
        	char* chat = strtok(NULL,"\n");
        	sprintf(sendData,"[%s`s whisper] %s",list[client_index].nickname,chat);
            
		for(i=0; i<MAX_CLIENT; i++){
              		if(list[i].c_socket!=INVALID_SOCK && strcmp(list[i].nickname,nickname)==0 && list[client_index].room==list[i].room){
                	write(list[i].c_socket,sendData,strlen(sendData));
              		}
        	}
        }

          else {
            	for(i=0; i<MAX_CLIENT; i++){
          		if(list[i].c_socket!=INVALID_SOCK && i!=client_index && list[client_index].room==list[i].room){
          			write(list[i].c_socket,sendData,strlen(sendData));
              		}
        	}
	}

          if(strstr(chatData, escape) != NULL) {
          	popClient(c_socket);
                break;
          }
        }
    }

int searchClient(int c_socket) {
	int i;
	int rst = -1;

	for(i = 0; i < MAX_CLIENT; i++)
		if(list[i].c_socket == c_socket)
			rst = i;
	return rst;
}

int pushClient(int c_socket) {
	int i=0;
	for(i=0; i< MAX_CLIENT; i++){
		if(list[i].c_socket==INVALID_SOCK) {
			list[i].c_socket=c_socket;
			return i;
		}
	}
	return -1;
}
int popClient(int c_socket) {
	int i = 0;
	
	for(i = 0; i < MAX_CLIENT; i++) {
		if(list[i].c_socket == c_socket) {
			close(c_socket);
			list[i].c_socket = INVALID_SOCK;
			return -1;
		}
	}
  	return -1;
}
