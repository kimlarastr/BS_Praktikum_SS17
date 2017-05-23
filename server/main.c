#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


<<<<<<< HEAD
int main(int argc, char *argv[]){

=======
int main(int argc, char*argv[]){
  //Sockets Adressfamilien, evtl noch fuellen!
>>>>>>> 13ef3df56b5ae8a667b1e6f1b5883fc53e3f084b
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

<<<<<<< HEAD


=======
    int portnr=4207; //atoi(argv[1]); //Portnr auslesen (atoi = char to int)

    //Struct füllen
>>>>>>> 13ef3df56b5ae8a667b1e6f1b5883fc53e3f084b
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
<<<<<<< HEAD

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
=======
    client_len= sizeof(client);

    fileDescriptor= accept(sock, &client, &client_len); //Sock = Rendezvous descriptor, fileDescriptor =Verbindungsdiscriptor

    structsockaddr_inclient;// Socketadresseeines Clients
    int fd; // Filedeskriptor für das Socket
    int client_len;// Laenge der Client-Daten
    char in[2000];// Daten vom Client an den Server (laenge der Nachricht)
    char out[2000];// Daten vom Server an den Client
    client_len= sizeof(client);  // Groeße der Client-Nachricht

    while (TRUE){   //warten auf Anfrage
        fd = accpet(sock, &client, &client_len);
        while (read(fd, in, 2000)>0){ //Daten vom Socket ==> in
            //Clientdaten
            write (fd, out,2000);} //Daten von out ins Socket
            close(fd); //Client sendet keine Daten mehr
>>>>>>> 13ef3df56b5ae8a667b1e6f1b5883fc53e3f084b
    }
return 0;

} //richtige
