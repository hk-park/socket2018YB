/*
[추가 할것]
클라이언트가 나갈때 나갔다는 메시지 출력으로 바꾸기
*/
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
//pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
//init을 할 때 넣어줌 init을 않하면 ^^^^^^^^ 값을 넣어준다
pthread_mutex_t mutex2;

#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000

int list_c[MAX_CLIENT]; //접속한 클라이언트를 관리하는 배열
char escape[] = "exit";
char greeting[] = "Welcome to chatting room\n";
char CODE200[] = "Sorry No More Connection\n";
int client_count = 0;//list_c의 배열 위치
int thr_id;

int main(int argc, char *argv[])
{
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int len;
    int i, j, n;
    int res;
	/*mutex를 사용하기 위해선 attr값을 넣어 주어야 한다. 위에서 미리 넣으면 한해도 되나 이것은 에러 처리를 위함*/
    if(pthread_mutex_init(&mutex2, NULL) != 0) {
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
	memset(list_c, INVALID_SOCK, sizeof(list_c));//'-1'값은 클라이언트가 없는 뜻이다.
    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
        res = pushClient(c_socket); //접속한 클라이언트를 list_c에 추가
        if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
           write(c_socket, greeting, strlen(greeting));
			/*배열 위치가 1에 위치하는 데 1에는 값이 없다.*/
			thr_id = pthread_create(&thread, NULL, do_chat, (void *)&list_c[client_count -1]);
           //pthread_create with do_chat function.
        }
    }
}
void *do_chat(void *arg){
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
    int i, n;
    while(1) {
		memset(chatData, 0, sizeof(chatData));
       if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
			//client_count가 최대일때 i는 9까지 올라간다. 0~9의 개수는 10이다.
			for(i = 0; i < client_count; i++){
				//문제 발생 안될시 mutex 봉인
				pthread_mutex_lock(&mutex2);
				write(list_c[i], chatData, strlen(chatData));
				pthread_mutex_unlock(&mutex2);
			}
            //write chatData to all clients
			/*클라이언트가 exit를 보내면은 실행이된다.*/
			/*배열의 위치가 1인데 1에는 값이 없다.*/
            if(strstr(chatData, escape) != NULL) {
                popClient(list_c[client_count-1]);
                break;
            }
        }
    }
}
int pushClient(int c_socket) {
	//list_c의 주소는  0~9 까지 이므로 10은 들어 가면 안된다., client_count는 10끼지 올라감 이걸 사용해서 하려면 -1 해주기
	if(client_count < 10){
		list_c[client_count] = c_socket;
		//다음 위치, 1명 추가
		pthread_mutex_lock(&mutex2);
		client_count++;
		pthread_mutex_unlock(&mutex2);
		return client_count;
	}else{
		//list_c가 가득차있을 때
		return -1;
	}

    //ADD c_socket to list_c array.
    //
    ///////////////////////////////
    //return -1, if list_c is full.
    //return the index of list_c which c_socket is added.
}
int popClient(int c_socket){
	close(c_socket);
	list_c[client_count -1] = INVALID_SOCK;/*그 배열의 위치에다가 -1값을 넣어준다.*/
	//1명 나감
	pthread_mutex_lock(&mutex2);
	client_count--;
	pthread_mutex_unlock(&mutex2);
	return 0;//나중을 위하여
    //REMOVE c_socket from list_c array.
    //
    ///////////////////////////////////
}
