#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> //?
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <netdb.h>
#include <arpa/inet.h>

//for Select
#include <sys/time.h>
#include <unistd.h>  //?
#include <sys/ioctl.h> 

#define SERVPORT 6000 
#define BACKLOG 10 // for listen
#define BUFFER_SIZE 4096
			  
int main() 
{ 
    int server_socket,cilent_socket; //,serverudp_fd; 
    int sin_size;  //sizeof sin_size   
    struct sockaddr_in host_addr; 
    struct sockaddr_in remote_addr; 
	
	char buffer[BUFFER_SIZE];

	//Select 
	fd_set readfds,testfds;
	int ret, return_size;
	int fd, max_fd;
	int nread;
	
    /*init the socket*/
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{ 
		perror("Error: Socket()"); 
		exit(1); 
    } 
    host_addr.sin_family=AF_INET; 
    host_addr.sin_port=htons(SERVPORT); 
    host_addr.sin_addr.s_addr = INADDR_ANY; 
    bzero(&(host_addr.sin_zero),8); 
	
	sin_size = sizeof(struct sockaddr_in); 
	//bind 
	if (bind(server_socket, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1) 
	{ 
		perror("Error: Bind()"); 
		exit(1); 
    } 
	//listen
	if (listen(server_socket, BACKLOG) == -1) 
	{ 
		perror("Error: Listen()"); 
		exit(1); 
    } 

	FD_ZERO(&readfds);
	FD_SET(server_socket,&readfds);
	max_fd = server_socket;
	
    while(1) 
    { 	
		//at what condition, we exit????  quit the server
		if(ServerQuit==ClientNumber)
			break;
		else
		{
		   printf("%d  :  %d  \n",ServerQuit,ClientNumber);
		}
		
	   //consider   the  SEQ fo the folloing THREE part!!!

	   
	   testfds=readfds;
	   
	   ret = select(max_fd + 1,&testfds,(fd_set *)0,(fd_set *)0,(struct timeval *)0);
	   if(ret<1)
	   {
		   perror("Error: Select()");
		   exit(1);
	   }
	   
	for(fd = 0;fd < max_fd; fd++)
	{
		if(FD_ISSET(fd,&testfds))
		{		
			if(fd == server_socket)
			{	
				if ((cilent_socket = accept(server_socket, (struct sockaddr *)&remote_addr,(socklen_t *)&sin_size)) == -1) 
				{ 
					perror("Error: Accept()"); 
					continue; //?? vunerability??
				} 
			FD_SET(cilent_socket,&readfds);
			if (client_socket > max_fd)
			{
				max_fd = client_socket;
			}
#ifdef DEBUG
	printf("Received a connection from %s\n", inet_ntoa(remote_addr.sin_addr));  
#endif
		}
        else
		{
			//wait here to get message from the client, why close fd
           ioctl(fd, FIONREAD, &nread);  
		   if(nread==0)
		   {
			  close(fd);
			  FD_CLR(fd,&readfds);
	  		}
		   else
		   {	
			if((return_size=recv(fd, buffer, BUFFER_SIZE, 0)) ==-1)
			{
				perror("Error: Receive");
			}
#ifdef DEBUG
	puts(buffer);
#endif 		
			if (send(fd, buffer, BUFFER_SIZE, 0) == -1) 
			{
				perror("fail to send message");
			}

		}
	  }
	}
   }
   close(server_socket);
   
   return 0;
}

