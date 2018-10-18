#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
#define IPADDR "127.0.0.1"
#define BUFSIZE 10000
//#define SEP " "//strtok분리 기호.

int main(){
	int c_socket;
	struct sockaddr_in c_addr;
	int len;
	int n/*, ntok = 0, spa = -1, plag = 0, cmp*/;
	//char *token = NULL;
	//char *diff = NULL;
	//char *diff_2 = NULL;

	char rcvBuffer[BUFSIZ];
	char sendBuffer[BUFSIZ];
	//char tokenBuffer[BUFSIZ];
	
	c_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);
	
	if(connect(c_socket, (struct sockaddr *)&c_addr, sizeof(c_addr)) == -1){
		printf("Can not connect\n");
		close(c_socket);
		return -1;
	}
	
	while(1){
		//strcpy(sendBuffer, "Hi, I'm client\n");
		fgets(sendBuffer, sizeof(sendBuffer), stdin);
		sendBuffer[strlen(sendBuffer)-1] = '\0';
		write(c_socket, sendBuffer, strlen(sendBuffer));
		if(strncasecmp(sendBuffer, "quit", 4) == 0 || strncasecmp(sendBuffer, "kill server", 11) == 0){
			break;
		}
		/*if(strncasecmp(sendBuffer,"strlen",6) == 0){
		  strcpy(tokenBuffer, sendBuffer);
		  token = strtok(tokenBuffer, SEP);
		  while(token != NULL){
			if(strncasecmp(token,"strlen",6) == 0 && plag == 0){
				plag = 1;
			}
			else{
				ntok += strlen(token);
				spa++;
			}
			token = strtok(NULL, SEP);
		     }
		  plag = 0;
		  printf("[%s]의 문자열 개수: %d 띄어진부분 개수: %d\n",sendBuffer,ntok,spa);
		  ntok=0;
		  spa=-1;
		}*/
		
		/*if(strncasecmp(sendBuffer,"strcmp",6) == 0){
			strcpy(tokenBuffer, sendBuffer);
			token = strtok(tokenBuffer, SEP);
			while(token!=NULL){
				if(strncasecmp(token,"strcmp",6) == 0 && plag == 0){
					plag=1;
				}
				else{
					spa++;//매개 변수 들어온 개수
					if(spa == 0){
						diff = token;
					}
					else if(spa == 1){
						diff_2 = token;
					}
				}
				token = strtok(NULL, SEP);
			}
			plag=0;
			if(spa >= 2 || spa == -1 || spa == 0){
				printf("[ERR] 입력 방법이 잘못 되었습니다.\n");
				printf("입력방법: strcmp str1 str2\n");
			}
			else if(spa == 1){
				if(strcmp(diff, diff_2) == 0){
					cmp = 0;
					printf("[%s] 하고 [%s] 같습니다. :: cmp값: [%d]\n",diff,diff_2,cmp);
				}
				else if(strcmp(diff, diff_2) >= 1){
					cmp = 1;
					printf("[%s] 보다 [%s] 가 깁니다.. :: cmp값: [%d]\n",diff_2,diff,cmp);
				}
				else if(strcmp(diff, diff_2) <= -1){
					cmp = -1;
					printf("[%s] 보다 [%s] 작습니다. :: cmp값: [%d]\n",diff_2,diff,cmp);
				}
			}
			spa=-1;
		}*/
		
		if((n = read(c_socket, rcvBuffer, sizeof(rcvBuffer))) < 0){
			printf("[ERR] Cannot read\n");
			return -1;
		}
		
		rcvBuffer[n] = '\0';
		printf("received data length: %d\n", n);
		printf("received Data: %s\n", rcvBuffer);
	}
	close(c_socket);
}
