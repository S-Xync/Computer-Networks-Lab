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
  if(argc!=3){
    printf("The Input should be of the type below\n");
    printf("\n**********************************\n");
    printf("%s <option> <filename>\n",argv[0]);
    printf("**********************************\n\n");
    printf("There are two options : \nDownload\nUpload\n");
    return 0;
  }
  int csocket=0,n=0;
  int sending_buffer_size=1024;
  int receiving_buffer_size=1024;
  char sending_buffer[sending_buffer_size];
  char receiving_buffer[receiving_buffer_size];
  FILE *fp;
  char ch='#';
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
  if((n=read(csocket,receiving_buffer,sizeof(receiving_buffer)-1))>0){
    if(n<0){
      printf("Error recieving from server\n");
    }
  }
  receiving_buffer[n]='\0';
  printf("%s\n",receiving_buffer);//receives date and time

  //downloading code
  if(strcasecmp(argv[1],"download")==0){
    strcpy(sending_buffer,"download");
    write(csocket,sending_buffer,strlen(sending_buffer));
    printf("The file to download is : %s\n",argv[2]);
    strcpy(sending_buffer,argv[2]);
    write(csocket,sending_buffer,strlen(sending_buffer));
    fflush(stdout);
    if((n=read(csocket,receiving_buffer,sizeof(receiving_buffer)-1))>0){
      if(n<0){
        printf("Error recieving from server\n");
      }
    }
    receiving_buffer[n]='\0';
    if(strcmp(receiving_buffer,"yes")==0){
      printf("File %s available on server\n",argv[2]);
      printf("Downloading......\n");
      fflush(stdout);
      if(fp=fopen(argv[2],"w")){
        printf("File %s created successfully\n",argv[2]);
        fflush(stdout);
        while((n=read(csocket,receiving_buffer,sizeof(receiving_buffer)-1))>0){
          if(n<0){
            printf("Error recieving from server\n");
          }
          ch=receiving_buffer[n-1];
          receiving_buffer[n-1]='\0';
          fprintf(fp,"%s",receiving_buffer);
          fflush(fp);
          if(ch=='#'){
            printf("#");
            fflush(stdout);
          }else{
            printf("File %s downloaded successfully\n",argv[2]);
            fflush(stdout);
            break;
          }
        }
        fclose(fp);
      }else{
        printf("Unable to create file %s\n",argv[2]);
        fflush(stdout);
      }
    }else{
      printf("File %s not available on server\n",argv[2]);
      fflush(stdout);
    }

  }

  //uploading code
  else if(strcasecmp(argv[1],"upload")==0){
    strcpy(sending_buffer,"upload");
    write(csocket,sending_buffer,strlen(sending_buffer));
    printf("The file to upload is : %s\n",argv[2]);
    strcpy(sending_buffer,argv[2]);
    write(csocket,sending_buffer,strlen(sending_buffer));
    fflush(stdout);
    if((n=read(csocket,receiving_buffer,sizeof(receiving_buffer)-1))>0){
      if(n<0){
        printf("Error recieving from server\n");
      }
    }
    receiving_buffer[n]='\0';
    if(strcmp(receiving_buffer,"yes")==0){
      printf("File %s created in server\n",argv[2]);
      printf("Uploading......\n");
      fflush(stdout);
      if(fp=fopen(argv[2],"r")){
        printf("File %s opened successfully in reading mode\n",argv[2]);
        fflush(stdout);
      }else{
        printf("Unable to open file %s in reading mode\n",argv[2]);
        fflush(stdout);
      }
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
        write(csocket,sending_buffer,strlen(sending_buffer));
        if(ch==EOF){
          break;
        }
      }
      printf("Successfully uploaded %s to the server\n",argv[2]);
      fflush(stdout);
    }else{
      printf("File %s not created in server\n",argv[2]);
    }
  }

  //not upload or download
  else{
    printf("Not able to understand your option\n");
    printf("\nThe Input should be of the type below\n");
    printf("\n**********************************\n");
    printf("%s <option> <filename>\n",argv[0]);
    printf("**********************************\n\n");
    printf("There are two options : \nDownload\nUpload\n");
    return 0;
  }
  fflush(stdout);
  close(csocket);
  return 0;
}
