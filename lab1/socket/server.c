#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<ctype.h>
#define SERV_PORT 4444 
#define MAXLINE 300
#define LISTENQ 100
#define SA struct sockaddr   
void str_echo(int );
int main(int argc, char **argv)
{ 
   int listenfd, connfd;
   pid_t childpid;
   socklen_t clilen;
   struct sockaddr_in cliaddr, servaddr;
   listenfd=socket(AF_INET,SOCK_STREAM,0);
   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family=AF_INET;
   
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(SERV_PORT);
   bind(listenfd, (SA *) &servaddr , sizeof(servaddr));
   listen(listenfd , LISTENQ );
   for(;;)
    {
     clilen = sizeof(cliaddr);
     connfd = accept(listenfd , (SA *) &cliaddr, &clilen);
     if((childpid=fork())==0)
        {
        close(listenfd);
        str_echo(connfd);
        exit(0);
        }
        close(connfd);
     }
 } 
void str_echo(int sockfd)
 { 
  int i;
  char line[MAXLINE];
  FILE *fpin,*fpout;
  fpin=fdopen(sockfd,"r");
  fpout=fdopen(sockfd,"w");
  for(;;)
     {
    if(fgets(line,MAXLINE,fpin)==0)
     { printf("\n please again send :\n");return;}
    printf("\nthe client's  message : %s\n  ",line);
    for(i=0;i<strlen(line);i++)
         { 
          line[i]=toupper(line[i]);
         }  
    fputs(line,fpout);
    fflush(fpout);    
     }
}
