#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "Hi, I'm server\n";
 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n,get;
	int rcvLen;
	char rcvBuffer[100];
	char *token1,*token2;
	char l[100];
	char filereader[100];
	FILE* fp;
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
 
	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}
 
	if(listen(s_socket, 5) == -1) {
		printf("listen Fail\n");
		return -1;
	}
 	
	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			
			printf("[%s] received\n", rcvBuffer);
		if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			else if(strncasecmp(rcvBuffer,"안녕하세요",5)==0)
				strcpy(l,"안녕하세요. 만나서 반가워요.\n");
			else if(strncasecmp(rcvBuffer,"이름이 뭐야?",7)==0)
				strcpy(l,"나의 이름은 AAA야\n");
			else if(strncasecmp(rcvBuffer,"몇 살이야?",6)==0)
				strcpy(l,"나는 50살이야\n");
			else if(strncasecmp(rcvBuffer,"strlen",6)==0)
				sprintf(l,"%d\n",strlen(rcvBuffer)-7);
			else if(strncasecmp(rcvBuffer,"strcmp ",7)==0){
				char *token;
				char *str[3];
				int i = 0;
				token = strtok(rcvBuffer, " ");
				while(token!=NULL){
								str[i++] = token;
								token = strtok(NULL, " ");
				}
				if(i<3)
							sprintf(l, "문자열 비교를 위해서는 두 문자열이 필요합니다\n");
				else if(strcmp(str[1], str[2]))
							sprintf(l, "%s와 %s는 같은 문자열입니다.\n",str[1],str[2]);
				else 
							sprintf(l, "%s와 %s는 다른 문자열입니다.\n",str[1],str[2]);	
				}
				n=strlen(l);
				write(c_socket,l,n);
			  if(strncasecmp(rcvBuffer,"readfile", 8)==0){
							char *token1;
							char *stt[2];
							int y = 0;
							token1 = strtok(rcvBuffer, " ");
							while(token1!=NULL){
											stt[y++] = token1;
											token1 = strtok(NULL, " ");
							}
							fp = fopen(stt[1], "r");
							if(fp){
								while(fgets(filereader, 100, (FILE*)fp))
									write(c_socket,filereader,strlen(filereader));
							}
							fclose(fp);
							
			 }
			 if(strncasecmp(rcvBuffer,"exec", 4)==0){
				 char *token2;
				 char *stp[3];
				 char sendmail[100];
				 int t = 0;
				 token2 = strtok(rcvBuffer, " ");
				 while(token2 != NULL){
					 			stp[t++] =token2;
								token2 = strtok(NULL, " ");
					}
					if(strncasecmp(stp[1],"mkdir",5)==0){
						int ret = system("mkdir %s",stp[2]);
						sprintf(sendmail,"디렉토리가 생성되었습니다.\n");
						write(c_socket,sendmail,strlen(sendmail));
					}
					else if(strncasecmp(stp[1],"ls",2)==0){
						int rec = system(system("ls>list.txt"));
						sprintf(sendmail,"리스트파일이 생성되었습니다.\n");
						write(c_socket,sendmail,strlen(sendmail));
					}
			 }
			 
			n=strlen(buffer);
			write(c_socket,buffer, n);
		
	}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
}	
	close(s_socket);
}
