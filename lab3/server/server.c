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
  int sending_buffer_size=1024;
  int receiving_buffer_size=1024;
  char sending_buffer[sending_buffer_size];
  char receiving_buffer[receiving_buffer_size];
  int n=0;
  FILE *fp;
  char ch;
  time_t time_now;
  memset(sending_buffer,'0',sizeof(sending_buffer));
  memset(receiving_buffer,'0',sizeof(receiving_buffer));
  int r = rand() % 100;//random int between 0 and 99
  if(connection){
    printf("Connected to the client No : %d\n",r);
  }
  time_now=time(NULL);
  snprintf(sending_buffer,sizeof(sending_buffer),"No : %d\nTime --> %.24s\r\n",r,ctime(&time_now));
  write(connection,sending_buffer,strlen(sending_buffer));//sends date and time
  if((n=read(connection,receiving_buffer,sizeof(receiving_buffer)-1))>0){
    if(n<0){
      printf("Error receiving from client No : %d",r);
    }
  }
  receiving_buffer[n]='\0';

  //downloading code
  if(strcmp(receiving_buffer,"download")==0){
    printf("Downloading process Begins..\n");
    fflush(stdout);
    if((n=read(connection,receiving_buffer,sizeof(receiving_buffer)-1))>0){
      if(n<0){
        printf("Error receiving from client No : %d",r);
      }
    }
    receiving_buffer[n]='\0';
    if(fp=fopen(receiving_buffer,"r")){
      printf("Opened file %s in reading mode for client No : %d\n",receiving_buffer,r);
      strcpy(sending_buffer,"yes");
      write(connection,sending_buffer,strlen(sending_buffer));
      fflush(stdout);
      int i=0;
      while(1){
        for(i=0;i<sending_buffer_size-10;i++){
          ch=getc(fp);
          if(ch!=EOF){
            sending_buffer[i]=ch;
          }else if(ch==EOF){
            break;
          }
        }
        if(ch==EOF){
          sending_buffer[i]='-';
          sending_buffer[i+1]='\0';
        }else{
          sending_buffer[i]='#';
          sending_buffer[i+1]='\0';
        }
        write(connection,sending_buffer,strlen(sending_buffer));
        if(ch==EOF){
          break;
        }
      }
      printf("Successfully sent the requested file to the client No : %d",r);
      fflush(stdout);
    }else{
      printf("Unable to open the file %s in reading mode\n",receiving_buffer);
      strcpy(sending_buffer,"no");
      write(connection,sending_buffer,strlen(sending_buffer));
      fflush(stdout);
    }
  }

  //uploading code
  else if(strcmp(receiving_buffer,"upload")==0){
    printf("Uploading process begins by client NO :  %d\n",r);
    fflush(stdout);
    if((n=read(connection,receiving_buffer,sizeof(receiving_buffer)-1))>0){
      if(n<0){
        printf("Error receiving from client No : %d",r);
      }
    }
    receiving_buffer[n]='\0';
    if(fp=fopen(receiving_buffer,"w")){
      printf("Created file %s in writing mode by client No : %d\n",receiving_buffer,r);
      strcpy(sending_buffer,"yes");
      write(connection,sending_buffer,strlen(sending_buffer));
      fflush(stdout);
      while((n=read(connection,receiving_buffer,sizeof(receiving_buffer)-1))>0){
        if(n<0){
          printf("Error recieving from client No : %d\n",r);
        }
        ch=receiving_buffer[n-1];
        receiving_buffer[n-1]='\0';
        fprintf(fp,"%s",receiving_buffer);
        fflush(fp);
        if(ch=='#'){
          printf("#");
          fflush(stdout);
        }else{
          printf("File uploaded successfully by client No : %d\n",r);
          fflush(stdout);
          break;
        }
      }
      fclose(fp);

    }else{
      printf("Coundn't create file %s in writing mode for client No : %d\n",receiving_buffer,r);

    }
  }
  fflush(stdout);
  close(connection);
  return 0;
}







// strcpy(sending_buffer,"\nDo you want to :\n1. Upload\n2. Download\nGive Input : ");
// write(connection,sending_buffer,strlen(sending_buffer));//sends options
// if((n=read(connection,receiving_buffer,sizeof(receiving_buffer)-1))>0){
//   if(n<0){
//     printf("Error receiving from client No : %d",r);
//   }
// }
// receiving_buffer[n]='\0';
// printf("%s\n",receiving_buffer);//receives user option
// printf("sdygf\n");
