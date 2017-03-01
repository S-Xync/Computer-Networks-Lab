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
  int sending_buffer_size=512;
  int receiving_buffer_size=512;
  char sending_buffer[sending_buffer_size];
  char receiving_buffer[receiving_buffer_size];
  int n=0;
  FILE *fp;
  char ch;
  time_t time_now;
  memset(sending_buffer,'0',sizeof(sending_buffer));
  memset(receiving_buffer,'0',sizeof(receiving_buffer));
  printf("Sending process Begins..\n");
  fflush(stdout);
  if(fp=fopen("server.txt","r")){
    while(1){
      int i=0;
      for(i=0;i<400;i++){
        ch=getc(fp);
        if(ch!=EOF){
          sending_buffer[i]=ch;
        }else if(ch==EOF){
          break;
        }
      }
      sending_buffer[i]='\0';
      write(connection,sending_buffer,strlen(sending_buffer));
      //acknowledgement
      if((n=read(connection,receiving_buffer,sizeof(receiving_buffer)-1))>0){
        if(n<0){
          printf("Error receiving from client\n");
        }
      }
      receiving_buffer[n]='\0';
      if(n==2){//no length is 2
        fflush(stdout);
        close(connection);
        return 0;
      }
    }
  }else{
    printf("Unable to open the file server.txt in reading mode\n");
    fflush(stdout);
    close(connection);
    return 0;
  }
  fflush(stdout);
  close(connection);
  return 0;
}
