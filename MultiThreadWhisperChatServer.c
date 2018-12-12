#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

void *do_chat(void *);
int pushClient(int);
int popClient(int);

pthread_t thread;
pthread_mutex_t mutex;

#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000

struct userlist{
        int list_c;
        char client[30];
};

struct userlist ulist[MAX_CLIENT];
char escape[ ] = "exit";
char greeting[ ] = "Welcome to chatting room\n";
char CODE200[ ] = "Sorry No More Connection\n";

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
        ulist[i].list_c = INVALID_SOCK;
    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
        res = pushClient(c_socket);

        if(res < 0) {
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
            pthread_create(&thread, NULL, do_chat, (void*)&c_socket);
        }
    }
}

void *do_chat(void *arg)
{
    int i, n;
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
    char *client, *message, *receive;
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
                client = strtok(chatData," ");
                receive = strtok(NULL, "/w ");
                message = strtok(NULL, " ");

                for(i=0; i<MAX_CLIENT; i++){
			if(strcmp(ulist[i].client, receive) == 0){
                                sprintf(chatData, "[%s] %s",ulist[i].client,message);
                                write(ulist[i].list_c, chatData, strlen(chatData));
                                break;
                        }
                        else if(message == NULL){
                                for(i=0; i<MAX_CLIENT; i++){	
                                        if(ulist[i].list_c != INVALID_SOCK){
                                                write(ulist[i].list_c, chatData, n);
                                        }
                                }
                        }
                }
        }
        if(strstr(chatData, escape) != NULL) {
                popClient(c_socket);
                break;
        }
    }
}

int pushClient(int c_socket) {
        int i, n;
        char client[CHATDATA];

        memset(client, 0, sizeof(client));

        for(i=0; i<MAX_CLIENT; i++){
                pthread_mutex_lock(&mutex);
                if(ulist[i].list_c == INVALID_SOCK){
                        ulist[i].list_c = c_socket;
                        if(read(c_socket, client, sizeof(client))>0)
                                strcpy(ulist[i].client, client);
                        printf("%d %s\n",ulist[i].list_c, ulist[i].client);
                        pthread_mutex_unlock(&mutex);
                        return i;
                }
                pthread_mutex_unlock(&mutex);
        }

        if(i == MAX_CLIENT)
                return -1;
}

int popClient(int c_socket){
        int i;
        close(c_socket);

        for(i=0; i<MAX_CLIENT; i++){
                pthread_mutex_lock(&mutex);
                if(c_socket == ulist[i].list_c){
                        ulist[i].list_c = INVALID_SOCK;
                        pthread_mutex_unlock(&mutex);
                        break;
                }
                pthread_mutex_unlock(&mutex);
        }
        return 0;
}
