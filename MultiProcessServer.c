#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
 #define PORT 9000
 //#define PORT 10000

 #define BUFSIZE 10000 
 // 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
 //char buffer[100] = "hello, world\n";
 char buffer[BUFSIZE] = "Hi, I'm server\n";

 main( )
 {
         int   c_socket, s_socket;
         struct sockaddr_in s_addr, c_addr;
         int   len;
         int   n;
         int rcvLen;
         char rcvBuffer[BUFSIZE];
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
    
          while(1) {                                       
          len = sizeof(c_addr);
	  c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
	  if((pid = fork())>0){
	}
