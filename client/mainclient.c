#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <arpa/inet.h>

void error(char *msg){
  perror(msg);
  exit(0);
}

int main(int argc, char*argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    //memset(&serv_addr, 0, sizeof(serv_addr));


    if (argc<3){  // 3 Argumente werden übergeben --> client.out localhost portno
      fprintf(stderr, "usage %s hostname port\n", argv[0]);
      exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //socket erstellen
    if (sockfd <0){
      error("Error! Öffnen des Socket");
    }

    server =gethostbyname(argv[1]); //Servername

    if (server == NULL){
      fprintf(stderr, "Error! Host nicht gefunden\n" );
      exit(0);
    }

    bzero((char*) &serv_addr, sizeof(serv_addr)); //server_addr leeren
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*)&serv_addr.sin_addr, server->h_length); //kopieren der Werte  von server
    serv_addr.sin_port = htons(portno);

    //serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    //Connect zum Server
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){ //sockfd weiß wo, serv_addr hat infos
      error("Fehler bei der Verbindung");
      exit(1);
    }

    int e =0;
    while(e==0){
    //Server response lesen
    bzero(buffer,256);
    //n = recv(socket, buffer, 256,0);
    n = read(sockfd, buffer, 255);
    if (n < 0) {
       error("ERROR reading socket");
       exit(1);
    }
    printf("Hallo1: %s\n", buffer); //wert ausgeben

    //nach Eingabe fragen, der Server liest sie dann
    printf ("Bitte nachricht eingeben:");
    bzero(buffer, 256);
    fgets(buffer,255, stdin); // wo wird die Eingabe gespeichert
    //Nachricht zum Server senden
    n = write( sockfd, buffer , strlen(buffer));
    //n= send(sockfd, buffer, 255,0);
    if (n<0){
      error("Error writing socket");
      exit(1);
    }
    printf("Hallo2: %s\n", buffer); //wert ausgeben

    if(buffer == "exit"){
      e = 1;

    }
}
    close(sockfd);
    return 0;
}
