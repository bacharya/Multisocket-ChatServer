#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
int main(int argc, char *argv[])
{
  int sockfd,portno,n;
  struct sockaddr_in serv_addr;
  struct in_addr addr;
  struct hostent *server;
  int flag=1;
  char *hostname, endname[25] = ".englab.juniper.net";
  char buff[256],*address,serv_add[50];
  hostname = (char *) malloc(1024*(sizeof(char)));
  hostname = argv[1];
  strcat(hostname,endname);
  printf("%s", hostname);
  //struct ifreq ifr;
  printf("\n This is Client ::: *******\n\nEnter the portno and server-add :\n");
  scanf("%d%s",&portno,&serv_add);
  sockfd = socket(AF_INET,SOCK_STREAM,0);

  if(sockfd<0)
  {
    printf("\nError on opening socket :");
  }

  inet_aton(serv_add, &addr);
  server = gethostbyname(hostname);
  if(server == NULL)
  {
    printf("\nNo such host ");
    exit(1);
  }

  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);
  address=(char *)inet_ntoa(serv_addr.sin_addr);
  printf("\nServer address : %s", address);
  if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
  {
    printf("\nError on connecting");
    exit(0);
  }
  while(flag){
    //n = send(sockfd, flag, sizeof(flag),0);
    printf("\nEnter the msg to send:\n");
    bzero(buff,256);
    //fgets(buff,255,stdin);
    scanf("%s",&buff);
    n = send(sockfd,buff, strlen(buff),0);
    if(n<0)
    {
      printf("\nError on writing to socket");
      exit(0);

    }
    bzero(buff,256);
    n = read(sockfd,buff,255);
    if(n<0)
    {
      printf("\nErropr reading ");
      exit(0);
    }
    printf("\n%s",buff);
    printf("\nDo you want to continue ? type 1 for yes and 0 fror no");
    scanf("%d",&flag);

  }
  return 0;
}
