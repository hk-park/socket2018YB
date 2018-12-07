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

#define MAX_NAME 50
#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000

struct user{
		int user_socket;
		char user_name[MAX_NAME];
};

struct user  list_c[MAX_CLIENT]; //접속한 클라이언트를 관리하는 배열
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
         list_c[i].user_socket = INVALID_SOCK;

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
             pthread_create(&thread,NULL,do_chat,(void *)&c_socket);
         }
     }
}

void *do_chat(void *arg)
{
     int c_socket = *((int *)arg);
     char chatData[CHATDATA];
	 char tempData[CHATDATA];
	 char senduser[MAX_NAME];
     int i, n;
	
	 char *whisper_user;

     while(1) {
        	 memset(chatData, 0, sizeof(chatData));
         	 if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
             strcpy(tempData, chatData);             
				
			 whisper_user = strtok(tempData,"/");
			 whisper_user = strtok(NULL,"/");
			 for(i=0; i<MAX_CLIENT;i++){
					if(list_c[i].user_socket==c_socket)
					strcpy(senduser,list_c[i].user_name);
			}

            printf("%s가 당신에게\n",whisper_user);

			for(i = 0; i < MAX_CLIENT; i++){
				if(list_c[i].user_socket !=	INVALID_SOCK &&
				 whisper_user !=NULL && !strcmp(list_c[i].user_name,whisper_user)){
					printf("%s가 %s으로 보낸 메세지\n",senduser,list_c[i].user_name);		
					write(list_c[i].user_socket,chatData,n);
				}else if(list_c[i].user_socket != INVALID_SOCK && whisper_user ==NULL){
						write(list_c[i].user_socket,chatData,n);
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
	
	int i,n;
	char user_name[MAX_NAME];
	
	memset(user_name, 0 ,sizeof(user_name));	
     //ADD c_socket to list_c array.
     
		for(i = 0; i<MAX_CLIENT; i++){
		pthread_mutex_lock(&mutex);
		if(list_c[i].user_socket == INVALID_SOCK){
			list_c[i].user_socket = c_socket;
			if((n = read(c_socket,user_name,sizeof(user_name))) > 0){
					strcpy(list_c[i].user_name,user_name);
			}
			printf("Connect socket number is %d name is %s\n",
						list_c[i].user_socket,list_c[i].user_name);
			pthread_mutex_unlock(&mutex);
			return i;
		}
		pthread_mutex_unlock(&mutex);
	}
		
	if (i == MAX_CLIENT)
		return -1;
     //return -1, if list_c is full.
     //return the index of list_c which c_socket is added.
}

int popClient(int c_socket)
{	
	 close(c_socket);

     int i;

	 for(i = 0; i<MAX_CLIENT; i++){
			pthread_mutex_lock(&mutex);
		if(c_socket == list_c[i].user_socket){
				list_c[i].user_socket = INVALID_SOCK;
				pthread_mutex_unlock(&mutex);
				break;
			}
			pthread_mutex_unlock(&mutex);
		}
		return 0;		
     //REMOVE c_socket from list_c array.
     ///////////////////////////////////
}

