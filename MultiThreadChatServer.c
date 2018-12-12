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

struct user {
	int list;
	char cName[20];
};

struct user us[MAX_CLIENT];
//int    list_c[MAX_CLIENT]; 접속한 클라이언트를 관리하는 배열
char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";

int main(int argc, char *argv[ ])
{
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
        us[i].list = INVALID_SOCK;
    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
        res = pushClient(c_socket); //접속한 클라이언트를 list_c에 추가
        
	if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
            //pthread_create with do_chat function.
            pthread_create(&thread, NULL, do_chat, (void*)&c_socket);
        }
    }
}


void *do_chat(void *arg)
{
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
    int i, n;
    char *cName, *msg, *rName;
    char *name;
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
		//write chatData to all clients
		cName = strtok(chatData, " ");
		rName = strtok(NULL, "/w ");
		msg = strtok(NULL, " ");
		
		for(i=0; i<MAX_CLIENT; i++) {
			if(!strcmp(us[i].cName, rName)) {
				sprintf(chatData, "[%s] %s", us[i].cName, msg);
				write(us[i].list, chatData, strlen(chatData));
				break;
			}
			else if(msg == NULL) {
				for(i=0; i<MAX_CLIENT; i++) {
					if(us[i].list != INVALID_SOCK)
						write(us[i].list, chatData, n);
				}
			}
		}

		if(strstr(chatData, escape) != NULL) {
                	popClient(c_socket);
                	break;
            	}
        }
    }
}

int pushClient(int c_socket) {
    //ADD c_socket to list_c array.
    //return -1, if list_c is full.
    //return the index of list_c which c_socket is added.
    int i, n;
    char cName[CHATDATA];
    memset(cName, 0, sizeof(cName));
    for(i=0; i<MAX_CLIENT; i++) {
    	pthread_mutex_lock(&mutex);
	if(us[i].list == INVALID_SOCK) {
		us[i].list = c_socket;
		if(read(c_socket, cName, sizeof(cName))>0)
			strcpy(us[i].cName, cName);
		printf("%d %s\n", us[i].list, us[i].cName);
		pthread_mutex_unlock(&mutex);
		return i;
	}
	pthread_mutex_unlock(&mutex);
    }
    if(i==MAX_CLIENT)
	return -1;
}

int popClient(int c_socket)
{
    close(c_socket);
    //REMOVE c_socket from list_c array.
    int i;
    for(i=0; i<MAX_CLIENT; i++) {
	pthread_mutex_lock(&mutex);
	if(c_socket == us[i].list) {
		us[i].list = INVALID_SOCK;
		pthread_mutex_unlock(&mutex);
		break;
	}
	pthread_mutex_unlock(&mutex);
    }
    return 0;
}
