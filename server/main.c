#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


int main(int argc, char *argv[]){

  struct sockaddr_in{
  short sin_family;
  unsigned short sin_port;
  struct in_addr sin_addr;
  char sin_zero[8];
  };

  struct in_addr{
  unsigned long s_addr;
  };


  int sock;
    sock = socket(AF_INET, SOCK_STREAM,0);
    if (sock <0){
        perror("creating stream socket");
        exit(2);
    };



    struct sockaddr_in server;
    server.sin_family= AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(4207);



    if (bind(sock, (struct sockaddr *)&server, sizeof(server))<0){

        perror("binding socket");
    };


    listen (sock,5);

    struct sockaddr client;
    int fileDescriptor, client_len;

	client_len= sizeof(client);

    fileDescriptor= accept(sock, &client, &client_len);


    int fd;

    char in[2000];
    char out[2000];


    while (TRUE){
        fd = accept(sock, &client, &client_len);
        while (read(fd, in, 2000)>0){

            write(fd, out,2000);}
            close(fd);
    }
return 0;

} //richtige
