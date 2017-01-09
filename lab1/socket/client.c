//clint.c

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#define SERV_PORT  4444		//port number
#define SERV_ADD  "127.0.0.1" // servers address
//#define SERV_ADD  "202.141.129.30"
//#define SERV_ADD  argv[1] // servers address
#define MAXLINE 300
void str_cli(FILE *,int );
int main(int argc, char **argv)
{
   int sockfd;
   pid_t childpid;
   struct sockaddr_in  servaddr;

   sockfd=socket(AF_INET,SOCK_STREAM,0);
   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family=AF_INET;
   servaddr.sin_port=htons(SERV_PORT);
   servaddr.sin_addr.s_addr=inet_addr(SERV_ADD);

   if((connect(sockfd,(struct sockaddr *) &servaddr,sizeof(servaddr)))<0)
   			// if server is down
	{
		printf("\ncann,t connect, may be server down :\n");
		exit(0);
	}
   printf("\n connection is established :\n ");

	for(;;)
   		str_cli(stdin,sockfd);

	exit(0);

 }


 void str_cli(FILE *fp,int sockfd)
  {
   FILE  *fpout,*fpin;
   char sendline[MAXLINE];
   char recvline[MAXLINE];
   fpout=fdopen(sockfd,"w");
   fgets(recvline,MAXLINE,fp);

   fputs(recvline,fpout);
   fflush(fpout);
   fflush(fpout);
   sleep(1);
   fpin=fdopen(sockfd,"r");

	if((fgets(sendline,MAXLINE,fpin))!=0)
   	{
      	 printf("\n the server's response :\n");
       	 fputs(sendline, stdout);
   	}
}
