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

typedef struct _nickname{
	char nickname[20];
}t_nickname;

int list_c[MAX_CLIENT]; //접속한 클라이언트를 관리하는 배열
t_nickname nickname[MAX_CLIENT];//클라이언트 닉네임 저장 클라이언트 위치대로
char escape[] = "exit";
char greeting[] = "Welcome to chatting room\n";
char CODE200[] = "Sorry No More Connection\n";
int client_count = 0;//list_c의 배열 위치
int nickname_count = 0;//닉네임의 배열위치 또한 특정 클라이언트 위치 구할때 사용
int thr_id;

int main(int argc, char *argv[])
{
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int len;
    int res;
	int i = 0;
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
			pthread_mutex_lock(&mutex2);
			read(list_c[client_count -1], nickname[nickname_count-1].nickname, sizeof(t_nickname));/*별명 넣기*/
           write(list_c[client_count -1], greeting, strlen(greeting));
			for(i = 0; i<nickname_count;i++){
				printf("%s\n", nickname[i].nickname);
			}
			pthread_mutex_unlock(&mutex2);
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
	char *token;
	char *str[5] = {NULL,};
	int str_count = 0;
    while(1) {
		memset(chatData, 0, sizeof(chatData));
       if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
			//client_count가 최대일때 i는 9까지 올라간다. 0~9의 개수는 10이다.
			if(!strncasecmp(chatData,"/w ", 3)){
				token = strtok(chatData, " ");
				while(token != NULL){
					str[str_count++] = token;
					printf("%s\n",str[str_count - 1]);
					token = strtok(NULL, " ");
				}
				if(str_count >= 4 || str_count <= 1){
					printf("잘못 입력 하셨습니다.\n");
					str_count = 0;
				}else{
					str_count = 0;
					for(i = 0; i<nickname_count;i++){
						printf("good::::%s:::::%s\n",nickname[i].nickname,str[1]);
						if(!strncasecmp(nickname[i].nickname,str[1],strlen(str[1]))){
							pthread_mutex_lock(&mutex2);
							write(list_c[i], str[2], strlen(str[2]));
							pthread_mutex_unlock(&mutex2);
							i = 0;
							break;
						}
					}
					i = 0;
					printf("[%s]가 없습니다.\n", str[1]);
				}
				
			}else{
				for(i = 0; i < client_count; i++){
					//문제 발생 안될시 mutex 봉인
					pthread_mutex_lock(&mutex2);
					write(list_c[i], chatData, strlen(chatData));
					pthread_mutex_unlock(&mutex2);
				}
			}
            //write chatData to all clients
			/*클라이언트가 exit를 보내면은 실행이된다.*/
			/*배열의 위치가 1인데 1에는 값이 없다.*/
			/*클라이언트가 0 1 2 3이 있는 데 2가 종료되면은 카운트는 3이 되므로 주소 3에 있는 클라이언트는 사용 못한다.*/
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
		nickname_count++;
		pthread_mutex_unlock(&mutex2);
		return client_count;
	}else{
		//list_c가 가득차있을 때
		pthread_mutex_lock(&mutex2);
		nickname_count--;
		client_count--;
		pthread_mutex_unlock(&mutex2);
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
	pthread_mutex_lock(&mutex2);
	list_c[client_count -1] = INVALID_SOCK;/*그 배열의 위치에다가 -1값을 넣어준다.*/
	strcpy(nickname[nickname_count-1].nickname, NULL);
	//1명 나감
	client_count--;
	nickname_count--;
	pthread_mutex_unlock(&mutex2);
	return 0;//나중을 위하여
    //REMOVE c_socket from list_c array.
    //
    ///////////////////////////////////
}
