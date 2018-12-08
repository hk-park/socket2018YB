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
#define MAX_NAME 20
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000
struct user{
		int c_socket;
		char user_name[MAX_NAME];
};

struct user list_c[MAX_CLIENT]; //접속한 클라이언트를 관리하는 배열

char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";
int numOfClient =0;
int main(int argc, char *argv[ ])
{
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int    len;
    int    i, j, n;
    int    res;
//	pthread_t pthread;	//하던대로...
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
        list_c[i].c_socket = INVALID_SOCK;
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
		//	printf("현재 접속자수: %d\n",checkClient()); //넘오브 클라이언트의 값은 이미 더해져있음
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
    char chatData[CHATDATA], tempData[CHATDATA];
	char checkUser[MAX_NAME];
	char *whisper_user, *message, *nickName;
    int i, n;
    while(1) {
		char whisperData[CHATDATA];
        memset(whisperData, 0, sizeof(whisperData));
        memset(chatData, 0, sizeof(chatData));	
        memset(tempData, 0, sizeof(tempData));	
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
			chatData[n]='\0'; 
			strcpy(tempData,chatData); //귓속말 검사용 데이터복사	
		nickName = strtok(tempData," ");  // "/w "를 잘라버리고 
			whisper_user = strtok(NULL,"/w "); 		
			message = strtok(NULL,"\0");	//메시지를		
		sprintf(whisperData,"%s %s",nickName, message);		
			 for(i=0; i<MAX_CLIENT;i++){
					if(list_c[i].c_socket == c_socket)	//소켓이 일치하면, 이름을 보낼 유저(checkUser)에 등록
					strcpy(checkUser,list_c[i].user_name);
			}
			for(i = 0; i < MAX_CLIENT; i++){
				if(list_c[i].c_socket != INVALID_SOCK){
					if(message != NULL && !strcmp(list_c[i].user_name,whisper_user)){
							write(list_c[i].c_socket,whisperData,n);  //귓속말 보내기
					}
					else if(message == NULL){
							write(list_c[i].c_socket,chatData,n);  //일반 보내기
					}
				}
			}
            //write chatData to all clients
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
		if(list_c[i].c_socket == INVALID_SOCK){
			list_c[i].c_socket = c_socket;
			if((n = read(c_socket,user_name,sizeof(user_name))) > 0){
					strcpy(list_c[i].user_name,user_name);
			}
			printf("%d번 소켓에 닉네임%s등록\n", list_c[i].c_socket,list_c[i].user_name);
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
    	int i=0;
		for(i; i<MAX_CLIENT; i++){
			if(list_c[i].c_socket == c_socket)
				list_c[i].c_socket = INVALID_SOCK;
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
			if(list_c[i].c_socket == INVALID_SOCK)
			n--;
	}
	return n;
}
