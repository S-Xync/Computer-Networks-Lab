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
#include <pthread.h> // for threading

void *connection_handler(void *);//function that handles each thread

int main(int argc, char const *argv[]) {
  int listening_sock=0,client_sock=0,c;
  struct sockaddr_in server_address,client_address;
  if((listening_sock = socket(AF_INET, SOCK_STREAM, 0))< 0){
    printf("\n Error : Could not create listening socket \n");
    return 1;
  }
  printf("Listening socket created successfully\n");
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
  c = sizeof(struct sockaddr_in);
  pthread_t thread_id;
  while((client_sock = accept(listening_sock, (struct sockaddr *)&client_address, (socklen_t*)&c))) {
    puts("\nConnection accepted");
    if(pthread_create( &thread_id, NULL,  connection_handler, (void*) &client_sock) < 0) {
      perror("could not create thread");
      return 1;
    }
    puts("Handler assigned");
  }
  if (client_sock < 0) {
    perror("accept failed");
    return 1;
  }
  return 0;
}


void* connection_handler(void* socket_desc){
  int connection =*(int*)socket_desc;
  char sending_buffer[1024];
  char receiving_buffer[1024];
  int n=0;
  time_t time_now;
  memset(sending_buffer,'0',sizeof(sending_buffer));
  memset(receiving_buffer,'0',sizeof(receiving_buffer));
  int r = rand() % 100;//random int between 0 and 99
  if(connection){
    printf("Connected to the client No : %d\n",r);
  }
  time_now=time(NULL);
  snprintf(sending_buffer,sizeof(sending_buffer),"No : %d\nTime --> %.24s\r\n",r,ctime(&time_now));
  write(connection,sending_buffer,strlen(sending_buffer));
  while((n=read(connection,receiving_buffer,sizeof(receiving_buffer)-1))>0){
    if(n<0){
      printf("Error receiving from client No : %d",r);
    }
    receiving_buffer[n]='\0';

    char new_string[1024];
    for(int i=0;i<n-1;i++){
      new_string[i]=receiving_buffer[n-i-2];
    }
    new_string[n-1]='\0';
    printf("client %d: %s",r,receiving_buffer);
    write(connection,new_string,strlen(new_string));
    for(int i=0;i<n;i++){
      receiving_buffer[i]=tolower(receiving_buffer[i]);
    }
    char subbuff[5];
    memcpy( subbuff, &receiving_buffer[0], 3 );
    subbuff[3] = '\0';
    if(strcmp(subbuff,"bye")==0){
      printf("\nClient Sent Bye. Dropped Client No : %d\n\n",r);
      printf("Waiting for connections!!\n");
      strcpy(sending_buffer,"\nSent Bye!! Dropping Connection.\nBye!!\n");
      write(connection,sending_buffer,strlen(sending_buffer));
      close(connection);
    }
  }
return 0;
}
