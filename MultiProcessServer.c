#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
#define BUFSIZE 10000

char buffer[BUFSIZE] = "Hi. I'm server\n";

main()
{
	int c_socket,s_socket;
	struct sockaddr_in s_addr,c_addr;
	int len;
	int n;
	int rcvLen;
	char rcvBuffer[BUFSIZE];
	char *ptr[BUFSIZE];
         
	memset(&s_addr,0,sizeof(s_addr));
        s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

  	s_socket=socket(PF_INET,SOCK_STREAM.0);
        if(bind(s_socket,(struct sockaddr *) &s_addr,sizeof(s_addr)) == -1){
		printf("Can not Bind\n");
		return -1;
	}
	if(listen(s_socket,5) == -1){
		printf("listen Fail\n");
		return -1;
	}

      
	while(1){
		c_socket=accept(s_socket,(struct sockaddr *)&c_addr,&len);
		if((pid = fork()) > 0){
                 	close(c_socket);
			continue;
		}else if(pid == 0){
			close(s_socket);
			do_service(conn_s);
			exit(0);
		}
	}
}
do service(int c_socket){
	while(1){
		
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr,&len);
		printf("Client is connected\n");
		while(1){
		    
}
} 	
