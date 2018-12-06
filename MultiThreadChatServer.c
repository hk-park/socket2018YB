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

int checkClient(); //클라이언트 인원수 측정
pthread_t thread;
pthread_mutex_t mutex;
#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000
int    list_c[MAX_CLIENT]; //접속한 클라이언트를 관리하는 배열
char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room";
char    CODE200[ ] = "Sorry No More Connection\n";
int numOfClient =0;
int main(int argc, char *argv[ ])
{
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int    len;
    int    i, j, n;
    int    res;
	pthread_t pthread;	//하던대로...
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
			numOfClient = res;
			//정상적으로 접속했을 경우
			printf("현재 접속자수: %d\n",checkClient()); //넘오브 클라이언트의 값은 이미 더해져있음
           write(c_socket, greeting, strlen(greeting));
			pthread_mutex_lock(&mutex);
		  	pthread_create(&thread, NULL, do_chat,(void *)&c_socket);
			pthread_mutex_unlock(&mutex);
            //pthread_create with do_chat function.
			
        }
    }
}
void *do_chat(void *arg)
{
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
    int i, n;
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
		 //	chatData[n-1]='\n'; //개행문자 부분을 끝 으로 바꿔줌 
			chatData[n-1]='\0'; //개행문자 부분을 끝 으로 바꿔줌 
			pthread_mutex_lock(&mutex);
			for(i=0;i<MAX_CLIENT;i++){
				if(list_c[i] != INVALID_SOCK){
					write(list_c[i], chatData, strlen(chatData));	
				}
			}
			pthread_mutex_unlock(&mutex);
			//클라이언트에게  내용 보냄	. 위에서 처리한 후 buffer에 strlen(buffer)의 크기만큼 넣은 내용물을 c_socket에 담아 보낸다. 

            //write chatData to all clients
            if(strstr(chatData, escape) != NULL) {
                popClient(c_socket);			
                break;
            }
        }
    }
}
int pushClient(int c_socket) {
    //ADD c_socket to list_c array.
	int i=0;
	int flag = 1;
	for(i; i<MAX_CLIENT; i++){
	if(list_c[i] == INVALID_SOCK){
		list_c[i] = c_socket;	
		flag = 0;
		break;	
		}		
	}
	if(flag)
		return -1;
	else
		return i+1;
    //return -1, if list_c is full.
    //return the index of list_c which c_socket is added.	
}
int popClient(int c_socket)
{
    	int i=0;
		for(i; i<MAX_CLIENT; i++){
			if(list_c[i] == c_socket)
				list_c[i]= INVALID_SOCK;
				break;		
		}
		numOfClient--;
    //REMOVE c_socket from list_c array.
	return 0;
}
int checkClient(){
	int i=0;
	int n=10;
	for(i; i<MAX_CLIENT; i++){
			if(list_c[i] == INVALID_SOCK)
			n--;
	}
	return n;
}
