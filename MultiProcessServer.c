#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>	
#include <signal.h>
#include <sys/wait.h>

#define PORT 12000
		
char buffer[100]="NONE";

int pid;
int numClient = 0; //현재 접속한 클라이언트 수
void sig_handler();
int s_socket;
int fd[2];
int processKill = 0;

int main(){

	int c_socket;
	int strLenCount=0,strcmpTarget=0;
	//s_socket: //클라이언트의 접속을 기다리는 소켓
	//c_socket: //클라이언트에게 메시지를 전달하기 위한 소켓
	struct sockaddr_in s_addr, c_addr;	//netinet.에 선언되어있는 구조체
  	int len,n;
		
	//11주차 실습 start
	int returnSignal;
	signal(SIGCHLD, sig_handler); // 자식 프로세스에서 신호가 오면 sig_handler 를 실행하겠다. 

	//11_22  11주차 실습  end
	//3-4

	//
	s_socket = socket(PF_INET, SOCK_STREAM, 0);//소켓 초기화
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IP주소 저장
	s_addr.sin_family = AF_INET;//
	s_addr.sin_port = htons(PORT); //몇번 포트를 사용할거냐
	//htons 일단은 컴퓨터가 이해할 수 이해할수있게 변환한다고생각

	if(pipe(fd)<0){
		printf("[ERR] pipe error");
		exit(1);
	}
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
		if(processKill)
			kill(-1*getpid(), SIGKILL); //부모, 자식 프로세스 모두 종료
		len = sizeof(c_addr);	
		c_socket = accept(s_socket, (struct sockaddr*)&c_addr, &len);//클라이언트가 접속할때까지 대기
	

		printf("현재 %d 명의 클라이언트 접속중 \n",++numClient); //1 더하고 출력 
					
		if((pid = fork()) > 0)  //부모 프로세스는 accept를 받으면 자신을 fork()해서 
		{
			close(c_socket);
			continue;		//s_socket이 남은 프로세스 하나 생성
		}
		else if(pid == 0) //자식 프로세스는 부모와 같은 정보를 가졌지만 s_socket을 지우고 c_socket 만으로 소통한다.
		{
			numClient = 0; //이거 안하면 자식프로세스가 서버 접속자수 정보를 가지고있는거 아닌가?
			close(s_socket);
			do_service(c_socket);		
			exit(0);
		}
			//분명 c_socket을 한번 더 닫는거같는데 오류가 안난다. 무시하는거같다. exit 해서그런가? 그럼 부모 프로세스는 뭐지?
							
		close(c_socket); //quit 사용시 c소켓 닫힘 
	}
	close(s_socket);
}

do_service(int c_socket)
{
	int rcvLen;
	char rcvBuffer[100]="N/A";
	while(1)
	{
		strcpy(buffer,"나는 서버입니다.(기본)");
		rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
		rcvBuffer[rcvLen-1]='\0'; //개행문자 부분을 끝 으로 바꿔줌 
		printf("recived: %s\n", rcvBuffer);
		if(strncasecmp(rcvBuffer,"kill server",11)==0)
		{	
			
			break;
		} 
		else if(strncasecmp(rcvBuffer,"quit",4)==0)
		{		
			printf("Client Disconnected\n");
			break; //accpet 로 돌아가기
		}
		else if(strcmp(rcvBuffer,"안녕하세요")==0)	//5-1 인사하기
			strcpy(buffer,"안녕하세요, 만나서 반가워요");
		else if(strncmp(rcvBuffer,"이름이 머",13)==0)			//5-2 이름이 야?
			strcpy(buffer,"내 이름은 비전, 생명의 편이죠");
		else if(strncmp(rcvBuffer,"strlen",6)==0)	//5-3 strlen
		{	
			//	strLenCount = strlen(rcvBuffer)-7; //"strlen " 만큼 빼버림				
			//	sprintf(buffer,"문자열 길이: %d",strLenCount); // 그리고 값을 반환함.
			sprintf(buffer,"문자열 길이: %d",strlen(rcvBuffer)-7); 
			//centOs7에선 한글 하나당 3바이트
		}
		else if(strncmp(rcvBuffer,"strcmp ",7)==0) // 5-3 strcmp
		{
			//printf("recived: %s\n", rcvBuffer);
			char *token;
			char *str[3];
			int i=0;
			token = strtok(rcvBuffer," ");
			while(token != NULL)
			{
				str[i++]= token;
				token = strtok(NULL," ");
			}		
			if(i<3)
				sprintf(buffer,"문자열이 두 개가 입력해야 합니다.");
			else if(!strcmp(str[1], str[2]))
				sprintf(buffer, "%s와 %s는 같은 문자열입니다.",str[1], str[2]);
			else
			{	
				i = strcmp(str[1],str[2]);
				sprintf(buffer, "%s와 %s는 다른 문자열입니다.[%d]",str[1], str[2],i);
			}
		} //else if strcmp
		//6-1 readme 실습
		else if(strncasecmp(rcvBuffer,"readfile",8)==0)
		{	
		//	printf("recived: %s\n", rcvBuffer);
			char *token;
			char *str[2];
			int i=0;
			token = strtok(rcvBuffer," ");
			while(token != NULL)
			{
				str[i++]= token;
				token = strtok(NULL," ");
			}	
			if(i>2) //인수가2 보다 많으면
				sprintf(buffer,"한번에 하나의 파일만 읽을 수 있습니다.");
			else if(i<2) //인수가 2보다 적으면
				sprintf(buffer,"파일명을 입력해주세요");
			else if(i==2) //인수가 2라면 (readme + test.txt)
			{
				FILE *fp;
				fp=fopen(str[1],"r");
				if(fp != NULL)
				{			
					sprintf(buffer,""); 
					char buffer2[100]="NONE";//버퍼를 하나 더만듬
					while(1)
					{															
						if(fgets(buffer2,sizeof(buffer2),(FILE*)fp))
						strcat(buffer,buffer2);
						else
							break;
					}	//fgets 가 불가능할때까지 하나씩 buffer에 strcat 해줌
				fclose(fp);
				}
				else
					sprintf(buffer,"파일을 찾을 수 없습니다");
			}//else if i==2
		}//else if readfile
		else if(strncasecmp(rcvBuffer,"exec",4)==0)
		{
			//printf("recived: %s\n", rcvBuffer); //서버용 리시버 (많이 있는데 얘만 주석있음)
			char *token;
			char *str[2];
			int i=0;
			token = strtok(rcvBuffer," ");
			while(token != NULL)
			{
				str[i++]= token;
				token = strtok(NULL,""); //\0도 가능함..
			}	
			if(i<2)
				sprintf(buffer,"명령 인수가 잘못되었습니다.");
			else
			{	
				int ret = system(str[1]); 
				if(!ret)
					sprintf(buffer,"%s\nCommand Success.",str[1]);
				else
					sprintf(buffer,"%s\nCommand Failed.",str[1]);
			}
		} //else if exec
	write(c_socket, buffer, strlen(buffer));	//클라이언트에게  내용 보냄	. 위에서 처리한 후 buffer에 strlen(buffer)의 크기만큼 넣은 내용물을 c_socket에 담아 보낸다. 
	}
	close(c_socket);
	write(fd[1],rcvBuffer,rcvLen); 
} //do_service(c_socket);

void sig_handler(){
	int pid; //pid
	int status; //자식 프로세스 상태
	pid = wait(&status);
	if(pid==-1){ //exec 전용, 끊지말고 리턴
		return;
	}else{ //아니라면 
		read(fd[0],buffer,sizeof(buffer));
		if(!strncasecmp(buffer, "kill server", 11)){
			printf("[INFO] pid[%d]로 부터 kill server 명령이 도착했습니다. 서버가 종료됩니다.\n", pid);
			close(s_socket);
			processKill=1;
		}else{
			printf("[INFO] pid[%d] is terminated. status = %d\n", pid, status);
			printf("[INFO] 1개의 클라이언트가 접속종료되어 %d개의 클라이언트가 접속되어 있습니다.\n", --numClient);
		}
	}
}



