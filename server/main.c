#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "function.h"

int main(int argc, char*argv[]){

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

printf("%s\n","socket ersellt" );
    //server unerwartet schlie�en
    int option = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &option, sizeof(int));

    int portnr=4207;
    struct sockaddr_in server;
    server.sin_family= AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(portnr);



    if (bind(sock, (struct sockaddr *)&server, sizeof(server))<0){
        perror("binding socket");
    };
printf("%s\n","socket gebunden" );

    listen (sock,5);

    struct sockaddr client;
          socklen_t client_len;
	  client_len= sizeof(client);

    int fd;
    char input[1024];
    char command[10]; //PUT GET DEL oder EXIT
    char key[30];
    char value[30];
    char res[2000];
    char* begruesung = "Verwenden sie bitte eine der Funktionen PUT(key value), GET(key); DEL(key) oder beenden sie die Kommunikation mit EXIT\n";
    char* teil;
    int e = 1;

    while (TRUE){
printf("%s\n","anfang 1 while schleife" );
        fd = accept(sock, &client, &client_len);
printf("%s\n","nach accept" );
        write(fd, begruesung,strlen(begruesung));
printf("nach begr��ung %d\n", (int)strlen(begruesung) );
        while (e=1){
	//bzero(input, 2000);
	       read(fd, input, 2000);
	       input[strlen(input)-2]='\0';

printf("%s\n","anfang 2 while schleife" );
          teil = strtok(input, " ");
printf("%s%d\n", teil, strlen(teil));
          strcpy(command,teil);
printf("%s%d\n", command, strlen(command));
          teil = strtok(NULL," ");
printf("%s%d\n", teil, strlen(teil));
          strcpy(key,teil);
printf("%s%d\n", key, strlen(key));
          teil = strtok(NULL, '\0');
printf("%s%d\n", teil, strlen(teil));
          teil = strcpy(value,teil);
printf("%s%d\n", value, strlen(value));
printf("Alle teile eingelesen");
fflush(stdout); //erzingt eine ausgabe

          //geht die command durch und vergleicht welche eingabe erfolgt ist
          printf("%s" , command);
          printf("%s", key);
          printf("%s", value);
          fflush(stdout); //erzingt eine ausgabe

           if(strcmp(command,"PUT")==0){
             put(key, value, res);
             printf("%s\n",res);
             write(fd, res,strlen(res));
           }else if(strcmp(command, "GET")==0){
             get(key, res);
             printf("%s\n",res );
             strcat(res,"\n"); //fügt zeilenumbruch bei
             write(fd, res,strlen(res));
           }else if(strcmp(command, "DEL")==0){
             del(key, res);
             printf("%s\n",res);
             strcat(res, "\n");
             write(fd, res,strlen(res));
           }else if(strcmp(command, "EXIT")==0){
             e = 0;
             close(fd);
           }else {printf("falsche eingabe\n");}

           //eingabe variablen leeren
           memset(command,0,sizeof(command));
           memset(key,0,sizeof(key));
           memset(value,0,sizeof(value));
           }
    }
return 0;

} //richtige
