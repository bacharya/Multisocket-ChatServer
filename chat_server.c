#include "utils.h"

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

static ret_code
communicate(int fd) {

    char recvbuff[SERVER_BUFF_MAX] = {0};
    char *terminate = "bye";
	char sendBuff[SERVER_BUFF_MAX] = {0};
	time_t ticks;
    int n;

	n = read(fd, recvbuff, SERVER_BUFF_MAX);
	if(n<0)
	{
		printf("\nError reading %d\n", fd);
		return RET_CODE_ERROR;
	}
	if(strcmp(recvbuff, terminate) == 0){
        return RET_CODE_GRACEFUL_EXIT;
	}
	ticks = time(NULL);
	printf("\nConnection fd: %d Message recieved at :%.24s Bytes:%d\n", fd, ctime(&ticks), n);
	snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));

    write(fd, sendBuff, strlen(sendBuff)); 

    return RET_CODE_SUCCESS;
}

int
main(int argc, char *argv[])
{
	int listenfd = 0, fd, portnumber;
    int epollfd, nfds, status, i;
    int ev_count=0, conn_count=0;
	struct sockaddr_in serv_addr; 
	char sendBuff[SERVER_BUFF_MAX];
    struct epoll_event ev, events[SERVER_MAX_EVENTS];
    ret_code rc;
	
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
    listen(listenfd, SERVER_CONN_MAX);

    /*Create Epoll instance*/
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
		printf("\nError !! epoll_create1\n");
        exit(EXIT_FAILURE);
    }

    /*Register interested events*/
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) == -1) {
        printf("\nError !!epoll_ctl: listen_sock\n");
        exit(EXIT_FAILURE);
    }

	while(1)
	{
        nfds = epoll_wait(epollfd, events, SERVER_MAX_EVENTS, -1);
        if (nfds == -1) {
            printf("\nError !! epoll_wait");
            exit(EXIT_FAILURE);
        }
        for(i=0;i<nfds;i++){
            if (events[i].data.fd == listenfd) {
                fd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
                if(fd <= 0) {
                    printf("\nError !! accept\n");
                    exit(EXIT_FAILURE);
                }
                conn_count++;
                printf("\nAccepted connection count: %d\n", conn_count);
                /*Set non-blocking socket*/
                status = fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
                if (status == -1) {
                    printf("\nError !! fcntl\n");
                    exit(EXIT_FAILURE);
                }
                ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
                ev.data.fd = fd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
                    printf("\nError !! epoll_ctl\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                ev_count++;
                printf("\nReceived event count: %d\n", ev_count);
                if(events[i].events && EPOLLRDHUP) {
                    printf("\nPeer closed connection : \n");
                    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &ev) == -1) {
                        printf("\nError !! epoll_ctl\n");
                        exit(EXIT_FAILURE);
                    }
                    close(events[i].data.fd);
                    conn_count--;
                }
                if(events[i].events && EPOLLIN) {
                    rc = communicate(events[i].data.fd);
                    printf("\nReturn code %d\n", rc);
                }
            }
        }
	}
    return 0;
}

