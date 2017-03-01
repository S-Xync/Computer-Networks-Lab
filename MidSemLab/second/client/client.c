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
  int sending_buffer_size=512;
  int receiving_buffer_size=512;
  char sending_buffer[sending_buffer_size];
  char receiving_buffer[receiving_buffer_size];
  FILE *fp;
  struct sockaddr_in server_address;
  memset(sending_buffer,'0',sizeof(sending_buffer));
  memset(receiving_buffer,'0',sizeof(receiving_buffer));
  if((csocket = socket(AF_INET, SOCK_STREAM, 0))< 0){
    printf("\n Error : Could not create socket \n");
    return 1;
  }
  printf("\nSocket created successfully\n");
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(5432);
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  if(connect(csocket, (struct sockaddr *)&server_address, sizeof(server_address))<0){
    printf("\n Error : Connection Failed \n");
    return 1;
  }
  printf("Connected to server\n");
  printf("Downloading......\n");
  fflush(stdout);
  if(fp=fopen("tsclient.txt","w")){
    printf("File tsclient.txt created successfully\n");
    fflush(stdout);
    while((n=read(csocket,receiving_buffer,sizeof(receiving_buffer)-1))>0){
      if(n<0){
        printf("Error recieving from server\n");
      }
      receiving_buffer[n]='\0';
      if(n==400){
        printf("Do you want to receive 50 more bytes (y=1 or n=0) :");
        int c=0;
        scanf("%d",&c);
        if(c==1){
          strcpy(sending_buffer,"yes");
        }else if(c==0){
          strcpy(sending_buffer,"no");
        }
      }else{
        strcpy(sending_buffer,"no");
      }
      write(csocket,sending_buffer,strlen(sending_buffer));
      fprintf(fp,"%s",receiving_buffer);
      fflush(fp);
      if(strlen(sending_buffer)==2){
        fclose(fp);
        printf("Sent NO to the server\n");
        close(csocket);
        fflush(stdout);
        return 0;
      }
    }
  }else{
    printf("Unable to create file %s\n",argv[2]);
    close(csocket);
    fflush(stdout);
    return 0;
  }
  fflush(stdout);
  return 0;
}
