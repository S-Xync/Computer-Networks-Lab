#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>//for time usage
#include <ctype.h>//for lower upper case usage

int main(int argc, char const *argv[]) {
  int listening_sock=0,connection=0;
  int n=0;
  struct sockaddr_in server_address;
  char sending_buffer[1024];
  char receiving_buffer[1024];
  time_t time_now;
  if((listening_sock = socket(AF_INET, SOCK_STREAM, 0))< 0)
  {
    printf("\n Error : Could not create listening socket \n");
    return 1;
  }
  // listening_sock=socket(AF_INET,SOCK_STREAM,0);
  printf("Listening socket created successfully\n");
  memset(sending_buffer,'0',sizeof(sending_buffer));
  memset(receiving_buffer,'0',sizeof(receiving_buffer));
  server_address.sin_family=AF_INET;
  server_address.sin_addr.s_addr=htonl(INADDR_ANY);
  server_address.sin_port=htons(5432);
  bind(listening_sock,(struct sockaddr*)&server_address,sizeof(server_address));
  if(listen(listening_sock,5)==-1){
    printf("Failed to listen\n");
    return -1;
  }
  printf("Running!!\n");
  printf("Waiting for connections!!\n");
  int count=0;
  while (1) {
    connection=accept(listening_sock,(struct sockaddr*)NULL,NULL);
    if(connection){
      count++;
      printf("\nConnected to a client No : %d\n",count);
      // printf(" );
    }
    // strcpy(sending_buffer,"You are connected to server\n");
    // write(connection,sending_buffer,strlen(sending_buffer));
    time_now=time(NULL);
    snprintf(sending_buffer,sizeof(sending_buffer),"Time : %.24s\r\n",ctime(&time_now));
    write(connection,sending_buffer,strlen(sending_buffer));
    // strcpy(sending_buffer,"You can send any text and it will echoed back\n");
    // write(connection,sending_buffer,strlen(sending_buffer));
    // strcpy(sending_buffer,"sending bye or word starting with bye will drop your connection\n\n");
    // write(connection,sending_buffer,strlen(sending_buffer));
    while((n=read(connection,receiving_buffer,sizeof(receiving_buffer)-1))>0){
      if(n<0){
        printf("Error receiving from client");
      }
      receiving_buffer[n]=0;
      printf("client : %s",receiving_buffer);
      // puts(receiving_buffer);
      // printf("client : %s",receiving_buffer);
      write(connection,receiving_buffer,strlen(receiving_buffer));
      for(int i=0;i<n;i++){
        receiving_buffer[i]=tolower(receiving_buffer[i]);
      }
      char subbuff[5];
      memcpy( subbuff, &receiving_buffer[0], 3 );
      subbuff[3] = '\0';
      // receiving_buffer[n-1]='0';
      // printf("after transform subbuff >%s<\n",subbuff);
      if(strcmp(subbuff,"bye")==0){
        printf("\nClient Sent Bye. Dropped Client No : %d\n\n",count);
        printf("Waiting for connections!!\n");
        strcpy(sending_buffer,"Sent Bye!! Dropping Connection.\nBye!!\n");
        write(connection,sending_buffer,strlen(sending_buffer));
        close(connection);
      }
    }
  }

  return 0;
}
