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
#include <ctype.h>//for lower upper case usage

int main(int argc, char const *argv[]) {
  int csocket=0,n=0;
  char sending_buffer[1024];
  char receiving_buffer[1024];
  struct sockaddr_in server_address;
  memset(sending_buffer,'0',sizeof(sending_buffer));
  memset(receiving_buffer,'0',sizeof(receiving_buffer));
  if((csocket = socket(AF_INET, SOCK_STREAM, 0))< 0)
  {
    printf("\n Error : Could not create socket \n");
    return 1;
  }
  printf("\nSocket created successfully\n");
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(5432);
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  // int connection=0;
  // connection=connect(csocket, (struct sockaddr *)&server_address, sizeof(server_address));
  // if(connection<0)
  if(connect(csocket, (struct sockaddr *)&server_address, sizeof(server_address))<0)
  {
    printf("\n Error : Connection Failed \n");
    return 1;
  }
  // printf("%d\n",connection);
  printf("Connected to server\n");
  if((n=read(csocket,receiving_buffer,sizeof(receiving_buffer)-1))>0){
    if(n<0){
      printf("Error receiving from server.\n");
    }
    receiving_buffer[n]=0;
    // printf("\nserver : \n");
    // puts(receiving_buffer);
    printf("server : %s\n",receiving_buffer);
  }
  printf("You can send any text and it will echoed back by server\n");
  printf("Sending bye or a word starting with bye will drop your connection\n\n");
  // for(int i=0;i<2;i++){
  //   if((n=read(csocket,receiving_buffer,sizeof(receiving_buffer)-1))>0){
  //     if(n<0){
  //       printf("Error receiving from server.\n");
  //     }
  //     receiving_buffer[n]=0;
  //     printf("\nserver : \n");
  //     puts(receiving_buffer);
  //     // printf("server : \n%s\n",receiving_buffer);
  //   }
  // }
  //testing
  // printf("hi i am out of loop\n");
  // if((n=read(csocket,receiving_buffer,sizeof(receiving_buffer)-1))>0){
  //   if(n<0){
  //     printf("Error receiving from server.\n");
  //   }
  //   receiving_buffer[n]=0;
  //   printf("\nserver : \n");
  //   puts(receiving_buffer);
  //   // printf("server : %s\n",receiving_buffer);
  // }
  char str[100];
  while(1){
    printf("client > ");
    // scanf("%s",str);
    // gets(str);
    fgets(str, sizeof(str), stdin );
    strcpy(sending_buffer,str);
    write(csocket,sending_buffer,strlen(sending_buffer));
    if((n=read(csocket,receiving_buffer,sizeof(receiving_buffer)-1))>0){
      if(n<0){
        printf("Error receiving from server.\n");
      }
      receiving_buffer[n]=0;
      printf("server : %s\n",receiving_buffer);
      // puts(receiving_buffer);
      // printf("server : %s\n",receiving_buffer);
    }
    for(int i=0;i<strlen(str);i++){
      str[i]=tolower(str[i]);
    }
    char subbuff[5];
    memcpy( subbuff, &receiving_buffer[0], 3 );
    subbuff[3] = '\0';
    if(strcmp(subbuff,"bye")==0){
      // close(connection);
      // printf("%d\n",connection);
      return 0;
    }
    // if((n=read(csocket,receiving_buffer,sizeof(receiving_buffer)-1))>0){
    //   if(n<0){
    //     printf("Error receiving from server.\n");
    //   }
    //   receiving_buffer[n]=0;
    //   printf("server : %s\n",receiving_buffer);
    // }
  }
  return 0;
}
