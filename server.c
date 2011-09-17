#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <netdb.h>
#include <arpa/inet.h>

#include <sys/time.h>
#include <unistd.h>  
#include <sys/ioctl.h> 

#define SERVPORT 9999
#define MAX_CLIENT 100 
#define BUFFER_SIZE 4096

//#define DEBUG

char buffer[MAX_CLIENT][BUFFER_SIZE];          
int  SocketToBuffer[MAX_CLIENT];
int  SocketReturnSize[MAX_CLIENT];

int  initBuffer()
{
    int i;
    for (i = 0; i< MAX_CLIENT; i++)
    {
        SocketToBuffer[i] = -1;
        SocketReturnSize[i] = -1;
        bzero(buffer[i], BUFFER_SIZE);
    }
    return 0;
}
int findFreeBuffer()
{
    int i;
    for (i = 0; i< MAX_CLIENT; i++)
    {
        if (SocketToBuffer[i] == -1)
        {
            return i;
        }
    }
    return -1;
}
int setReturnSize(int index, int size)
{
    return SocketReturnSize[index] = size;
}

int getReturnSize(int index)
{
    return SocketReturnSize[index];
}
int getBuffer(int index, int socket)
{
/* use the function, only if findFreeBuffer return a positive value*/
    SocketToBuffer[index] = socket;
    return socket;
}
int freeBuffer(int index)
{
    SocketToBuffer[index] = -1;
    SocketReturnSize[index] = -1;
    bzero(buffer[index], BUFFER_SIZE);
    return 0;
}
int findBuffer(int socket)
{
    int i;
    for (i = 0; i< MAX_CLIENT; i++)
    {
        if (SocketToBuffer[i] == socket)
        {
            return i;
        }
    }
    return -1;
}

int main() 
{ 
    int server_socket,client_socket;
    int sin_size;  
    struct sockaddr_in host_addr; 
    struct sockaddr_in remote_addr; 
    
    /*Select */
    fd_set readfds, writefds, testfds;
    int ret, return_size;
    int fd, max_fd;
    int index;
    
    initBuffer();
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

    if (bind(server_socket, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1) 
    { 
        perror("Error: Bind()"); 
        exit(1); 
    } 

    if (listen(server_socket, MAX_CLIENT) == -1) 
    { 
        perror("Error: Listen()"); 
        exit(1); 
    } 

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(server_socket, &readfds);

    max_fd = server_socket;
    
    while(1) 
    {     
	
        testfds=readfds;
       
        ret = select(max_fd + 1,&testfds, &writefds,(fd_set *)0, NULL);
        if(ret < 1)
        {
           perror("Error: Select()");
           exit(1);
        }
#ifdef DEBUG
    puts("select");  
#endif     
        for(fd = 0;fd <= max_fd; fd++)
        {
            if( FD_ISSET(fd, &testfds))
            {        
                if(fd == server_socket)
                {     
                    if ((client_socket = accept(server_socket, (struct sockaddr *)&remote_addr,(socklen_t *)&sin_size)) == -1) 
                    {     
                        perror("Error: Accept()"); 
                        continue; 
                    } 
#ifdef DEBUG
    puts("accept");  
#endif 
                    FD_SET(client_socket,&readfds); 
                    if (client_socket > max_fd)
                    {
                        max_fd = client_socket;
                    }                    
                }
                else
                {           
                    index = findFreeBuffer();
                    if (index == -1)
                    {
                        puts("No enough buffer");
                        continue; 
                    }
                    getBuffer(index, fd);
                    return_size = recv(fd, buffer[index], BUFFER_SIZE, 0);
                    if (return_size == -1)
                    {
                        index = findBuffer(fd);
                        freeBuffer(index);
                        FD_CLR(fd, &readfds);
                        FD_CLR(fd, &testfds);
                        FD_CLR(fd, &writefds);
                        close(fd);
                        fd = -1;
                        perror("Error: Receive");
                        continue;
                    }
                    if (return_size == 0)
                    {
#ifdef DEBUG
    printf("Client Exit: %d\n",fd);
#endif
                        index = findBuffer(fd);
                        freeBuffer(index);
                        FD_CLR(fd, &readfds);
                        FD_CLR(fd, &testfds);
                        FD_CLR(fd, &writefds);
                        close(fd);
                        fd = -1;
                        continue;
                     }    
                    setReturnSize(index, return_size);        
                    FD_SET(fd, &writefds);
#ifdef DEBUG
    printf("recv  fd is: %d ret: return_size: %d\n",fd ,return_size) ; 
    puts(buffer[index]);
#endif                         
                }
            }
            else if (FD_ISSET(fd, &writefds) )
            { 
                index = findBuffer(fd);
                if (index == -1)
                {
                    puts("Socket have no things to write");
                    continue; 
                }
                if ((return_size = send(fd, buffer[index], getReturnSize(index), 0)) == -1) 
                {
                    perror("fail to send message");
                }
                freeBuffer(index);
                FD_CLR(fd, &writefds);
#ifdef DEBUG
    printf("send  fd is: %d return_size: %d buffer numer: %d\n",fd, return_size, index) ;
    puts(buffer[index]);
#endif
            }
        }
    }
   close(server_socket);
   
   return 0;
}
