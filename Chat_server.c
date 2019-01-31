#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

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

   void memset(void *s, '0', sizeof(s))
   Used to initialize block of bytes of type sockaddr_in and char array to 0 (all memory location is set with zero '\0')

   int htons(int hostshort) :
   Used to convert port-number from host byte order to network byte order

   Char * inet_ntoa(in_addr) :
   Used to get server addres in byte order frmo network order 

   int bind(int sockfd, const struct sockaddr *addr, sizeof address_length )
   Used to bind socket to the server address and port. Here INADDR_ANY is used so that port is open to listen on all address configured on device.

   int accept(int sockfd, const struct sockaddr *addr, socklen_t addrlen) :
   Used to connect to clients requesting for connection on the specified port.

   ssize_t send(int sockfd, const void *buf, size_t len, int flags) :
   Used to send string of bytes to server. This function will return size of bytes it sent.

   ssize_t read(int fd, void *buf, size_t count) :
   Used to read string of bytes from the socket. This function will return size of bytes it read.  

   snprintf(char *buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks)) :
   Used to write local time to the buffer

   ssize_t write(int fd, const void *buf, size_t count) :
   Used to write buffer contents on the client connected socket 

   close(socket) :
   Used to close the socket

 */
struct client{

	int sockfd;
	struct client *next; 
};
typedef struct client *  node;

void push( struct client **, int );
void display( struct client **);
int check_fd( struct client **, int);
void close_connections(struct client  **);

int main(int argc, char *argv[])
{
	int listenfd = 0, connfd = 0, portnumber, n;
	struct sockaddr_in serv_addr; 
	char buff[256], terminate[] = "bye";
	struct client *Head = NULL;

	char sendBuff[1025];
	time_t ticks; 
	printf("\nThis is Chat Server:\n");
	printf("\nEnter the server port number : ");
	scanf("%d", &portnumber);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd < 0){
		printf("\nFailed to create Socket\n");
		exit(0);
	}

	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portnumber); 
	printf("\nBinding Socket to the Server Address:\n");
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

	printf("\nListening  on tcp port :%d\n", portnumber);
	listen(listenfd, 10); 

	while(1)
	{
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
		if(connfd > 0){
			break;
		} 
	}
	/*if(check_fd(&Head, connfd) > 0){
	  push(&Head, connfd);
	  display(&Head);
	  }*/
	// while(connfd != 0){
	//printf("\n%s",buff);
	while(1){
		bzero(buff,256);
		n = read(connfd,buff,255);
		if(n<0)
		{
			printf("\nError reading \n");
			exit(0);
		}
		if(strcmp(buff, terminate) == 0){
			break;
		}
		ticks = time(NULL);
		printf("\nMessage recieved at :%.24s",ctime(&ticks));
		snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
		write(connfd, sendBuff, strlen(sendBuff)); 
		sleep(1);
	}
	printf("\nClosing connection\n");
	close(connfd);
}

void push(struct client **head, int data){

	node ptr, temp;

	temp = *head;

	ptr = (node) malloc(sizeof(struct client));
	ptr->sockfd = data;
	ptr->next = NULL;

	printf("\nInserting to clients list\n");
	if(*head == NULL){

		*head = ptr;
		//	return head;
		return ;
	}
	while(temp->next != NULL){

		temp = temp->next;

	}

	temp->next = ptr;

	//return head;
	return ;
}

void display(struct client  **Head){

	node temp;
	temp = *Head;
	if(*Head == NULL){
		printf("\nThere are no clients connected\n");
		return;
	}
	printf("\nConnected Clients are :\n");
	while(temp->next != NULL){
		printf("%d ", temp->sockfd);
		temp = temp->next;
	}
	return ;
}

void close_connections(struct client  **Head){

	node temp;
	temp = *Head;
	if(*Head == NULL){
		printf("\nThere are no clients connected\n");
		return;
	}
	printf("\nConnected Clients are :\n");
	while(temp->next != NULL){
		printf("%d ", temp->sockfd);
		close(temp->sockfd);
		temp = temp->next;
	}
	return ;
}
int check_fd(struct client **Head, int data){

	node temp;
	int flag = 0;

	temp = *Head;
	printf("\nChecking clients list\n");
	if(*Head == NULL){
		return 1;
	}
	while(temp != NULL){
		if(temp->sockfd == data){
			flag = 1;
		}
		temp = temp->next;
	}
	if(flag == 0){

		return 1;

	} else{
		return -1;
	}

}





