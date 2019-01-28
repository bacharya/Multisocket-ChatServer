#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>


int main(int argc, char *argv[])
{
	/* 
	   In this module TCP client is implemented, tcp socket is created and Read values for server port and address.
	   Connecting to the server and start excahing messages.

	   Functions used in this module are:

	   socket(AF_INET,SOCK_STREAM,0) : 
	   Used to create TCP socket

	   int inet_aton(const char *cp, struct in_addr *inp) :
	   Used to convert Server address read as string to in_addr type (internet-address type)

	   void bzero(void *s, size_t n) :
	   Used to initialize server-address vaiable of type sockaddr_in to 0 (all memory location is set with zero '\0')

	   int htons(int hostshort) :
	   Used to convert port-number from host byte order to network byte order

	   Char * inet_ntoa(in_addr) :
	   Used to get server addres in byte order frmo network order 

	   int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) :
	   Used to connect to the specified TCP server

	   ssize_t send(int sockfd, const void *buf, size_t len, int flags) :
	   Used to send string of bytes to server. This function will return size of bytes it sent.

	   ssize_t read(int fd, void *buf, size_t count) :
	   Used ti read string of bytes from the socket. This function will return size of bytes it read.  

	 */

	int sockfd,portno,n;
	struct sockaddr_in serv_addr;
	struct in_addr addr;
	struct hostent *server;
	int flag=1;
	char buff[256],*address, serv_add[50];
	printf("\n This is Client ::: *******\n\nEnter the portno and server-add :\n");
	scanf("%d %s",&portno,serv_add);

	sockfd = socket(AF_INET,SOCK_STREAM,0);

	if(sockfd<0)
	{
		printf("\nError on opening socket :");
	}
	inet_aton(serv_add, &addr);

	bzero((char *)&serv_addr, sizeof(serv_addr));

	printf("\n Setting server address and port:\n");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr = addr;
	serv_addr.sin_port = htons(portno);

	address=(char *)inet_ntoa(serv_addr.sin_addr);

	printf("\nServer address : %s", address);

	if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
	{
		printf("\nError on connecting\n");
		exit(0);
	}
	while(flag){

		printf("\nEnter the msg to send:\n");
		bzero(buff,256);
		scanf("%s",buff);
		n = send(sockfd,buff, strlen(buff),0);
		if(n<0)
		{
			printf("\nError on writing to socket\n");
			exit(0);

		}
		bzero(buff,256);
		n = read(sockfd,buff,255);
		if(n<0)
		{
			printf("\nError reading \n");
			exit(0);
		}
		printf("\n%s",buff);
		printf("\nDo you want to continue ? type 1 for yes and 0 for no\n");
		scanf("%d",&flag);
	}
	return 0;
}
