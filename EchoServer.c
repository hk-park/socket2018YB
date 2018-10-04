#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>	
#define PORT 12000
		
char buffer[100]="NONE";
char tokBuffer[100]="";
char *token1,*token2,*token3;
		
int main(){

			int c_socket, s_socket;
			int strLenCount=0,strcmpTarget=0;
			//s_socket: //클라이언트의 접속을 기다리는 소켓

			//c_socket: //클라이언트에게 메시지를 전달하기 위한 소켓

			struct sockaddr_in s_addr, c_addr;	//netinet.에 선언되어있는 구조체


		  	int len,n;
		//3-4
			int rcvLen;
			char rcvBuffer[100]="N/A";
		//
			s_socket = socket(PF_INET, SOCK_STREAM, 0);//소켓 초기화
	
			memset(&s_addr, 0, sizeof(s_addr));
			s_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IP주소 저장
			s_addr.sin_family = AF_INET;//
			s_addr.sin_port = htons(PORT); //몇번 포트를 사용할거냐
				//htons 일단은 컴퓨터가 이해할 수 이해할수있게 변환한다고생각

			if(bind(s_socket,(struct sockaddr *)&s_addr,sizeof(s_addr))== -1 )
				{
					printf("[ERR] Cannot bind\n");
					return -1;
				}

			if(listen(s_socket, 5) == -1)
				{
					printf("[ERR] Cannot listen\n");
					return -1;
				}

			while(1)
			{	
					
					len = sizeof(c_addr);	
					c_socket = accept(s_socket, (struct sockaddr*)&c_addr, &len);//클라이언트가 접속할때까지 대기
					printf("Client is Connected\n"); 
		
					//3-4 
					while(1)
					{
						strcpy(buffer,"나는 서버입니다.(기본)");
						rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
						rcvBuffer[rcvLen-1]='\0';
						if(strncasecmp(rcvBuffer,"kill server",11)==0)
						{	
						printf("Server quit\n");
						close(c_socket);
						close(s_socket);
						return 0;	
						}
					else if(strncasecmp(rcvBuffer,"quit",4)==0)
					{		
						printf("Client Disconnected\n");
						break; //accpet 로 돌아가기
					}
					else if(strcmp(rcvBuffer,"안녕하세요")==0)	//5-1 인사하기
						strcpy(buffer,"안녕하세요, 만나서 반가워요");
					else if(strncmp(rcvBuffer,"이름이 머",8)==0||strncmp(rcvBuffer,"이름이 								뭐",8)==0)			//5-2 이름이 뭐야?
						strcpy(buffer,"내 이름은 비전, 생명의 편이죠");
					else if(strncmp(rcvBuffer,"strlen",6)==0)	//5-3 strlen
					{	
						strLenCount = strlen(rcvBuffer)-7; //"strlen " 만큼 빼버림				
						sprintf(buffer,"문자열 길이: %d",strLenCount); // 그리고 값을 반환함. 
						//centOs7에선 한글 하나당 3바이트
					}
					else if(strncmp(rcvBuffer,"strcmp",6)==0) // 5-3 strcmp
					{
						strcpy(tokBuffer,rcvBuffer);
						
						token1=strtok(tokBuffer," ");
					
							printf("%s\n", token1);							
							token1=strtok(NULL," ");
							token2=strtok(NULL," ");
							token3=strtok(NULL," ");
							printf("%s\n%s\n", token1,token2);	
					strcmpTarget = strcmp(token1,token2);
					sprintf(buffer,"반환: %d",strcmpTarget); // 그리고 값을 반환함. 
					}
					printf("recived: %s\n", rcvBuffer);
					write(c_socket, buffer, strlen(buffer));	//클라이언트에게  내용 보냄	
				
					}
		
				close(c_socket); //quit 사용시 c소켓 닫힘 
			}

	
		close(s_socket);



		}






