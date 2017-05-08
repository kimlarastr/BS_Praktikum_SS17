#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char*argv[])
{
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    //Verbindung mit dem Server herstellen
    connect (sock, &server, sizeof(server));

    //Name statt IP
    struct hostent *host_info;
    struct socaddr_in server;

    host_info =gethostbyname(argv[1]); //Servername
    memcpy (&server.sin_addr, host_info->h_addr, host_info->h_length);
    server.sin_port = htons(argv[1]);
    connect (sock, &server, sizteof(server));

}
