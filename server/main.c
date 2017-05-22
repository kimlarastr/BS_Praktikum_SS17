#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

/*
Fragen: createt with Socket(2, accepted with accept(2) <- socket wird erstellt. Was hat es mit der 2 auf sich? genauso in accept(2)


*/

int main(int argc, char*argv[]) // Portnr. & Ip adresse
{
  //Sockets Adressfamilien, evtl noch füllen!
  struct sockaddr_in{
  short sin_family;// AF_INET (Tag)
  unsigned shortsin_port;// TCP bzw. UDP Portnummer
  struct in_addrsin_addr;// 32-Bit IP-Adresse
  char sin_zero[8];// padding
  };
  struct in_addr{
  unsignedlongs_addr;// 32-Bit IP-Adresse
  };

  //Anlegen eines Sockets
  int sock;
    sock = socket(AF_INET, SOCK_STREAM,0); // (Domain, Typ, Protokoll)
    if (sock <0){
        perror("creating stream socket");
        exit(2);    //gibt einen Descriptor aus
    };

    int portnr=4207; //atoi(argv[1]); //Portnr auslesen (atoi = char to int)

    //Struct füllen
    struct sockaddr_in server;
    server.sin_familiy= AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(portnr);

    //An diese Adresse Socket binden

    if (bind(sock, (struct sockaddr *) &server, sizeof(server))<0){

        perror("binding socket");
    };

    //auf Verbindung warten

    listen (sock,5); //(int a, int backlog)bis zu 5 Anfragen in der Warteschlange

    //Verbindung akzeptieren
    struct sockaddr_in client;
    int fileDescriptor, client_len;
    client_len= sizeof(client);

    fileDescriptor= accept(sock, &client, &client_len); //Sock = Rendezvous descriptor, fileDescriptor =Verbindungsdiscriptor

    structsockaddr_inclient;// Socketadresseeines Clients
    int fd; // Filedeskriptor für das Socket
    int client_len;// Länge der Client-Daten
    char in[2000];// Daten vom Client an den Server (länge der Nachricht)
    char out[2000];// Daten vom Server an den Client
    client_len= sizeof(client);  // Größe der Client-Nachricht

    while (TRUE){   //warten auf Anfrage
        fd = accpet(sock, &client, &client_len);
        while (read(fd, in, 2000)>0){ //Daten vom Socket ==> in
            //Clientdaten
            write (fd, out,2000);} //Daten von out ins Socket
            close(fd); //Client sendet keine Daten mehr
    }

}
