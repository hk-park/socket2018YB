#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
#define MAX 100
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "Hi, I'm server\n";
 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int i;
	int rcvLen;
	char rcvBuffer[100];
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

			printf("[%s] received\n", rcvBuffer);
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			n = strlen(buffer);
			write(c_socket, buffer, n);
		}
//5-1
                        if(!strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요")))
                                strcpy(buffer, "안녕. 만나서 반가워");
//5-2
			
			if(!strncmp(rcvBuffer, "이름이 뭐야?", strlen("이름이 뭐야?")))
                                strcpy(buffer, "내 이름은 ooo.");	
			
			if(!strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?")))
				strcpy(buffer, "나는 22살");  	
			
//5-3_2
 			if(!strncasecmp(rcvBuffer,"strcmp",6)){
                                strcpy(buffer, rcvBuffer);
                                char *str[MAX];
                                str[0] = strtok(buffer," ");
                                str[1] = strtok(NULL, " ");
                                str[2] = strtok(NULL, "");
                                if(strcmp(str[1],str[2])==0 ){
                                        strcpy(buffer, "0-서로 같습니다.");
                                }
                         else
                                strcpy(buffer, "1_서로 다릅니다.");
                        }
 //5-3_1
			if(!strncasecmp(rcvBuffer,"strlen",6)){
				strcpy(buffer, rcvBuffer);
				char *str[MAX]; 
				str[0] = strtok(buffer, " ");
				str[1] = strtok(NULL, "" );
				n = strlen(str[1]);
                                write(c_socket, str[1], n);
				
			}
			else{
				n = strlen(buffer);
				write(c_socket, buffer, n);
			}
		}		
 		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}
}	