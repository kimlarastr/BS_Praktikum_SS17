#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
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
    int e=1;
    char shmid, *shar_mem, result; // Shared Memory Variablen

    //erzeugen und anlegen des Shared Memory
    shmid = shmget (IPC_PRIVATE, N, IPC_CREAT | 0644 );
    if (shmid == -1){
       printf ("Fehler bei key %d, mit der Größe %d\n",IPC_PRIVATE, N);
    }
    //anhängen/binden des Shared Memory
    //daten* shar_mem = (char*)shmat(shmid, NULL, 0);
    shar_mem = (char*)shmat(shmid, NULL, 0);
    if (shar_mem == (void *) -1)
      printf ("Fehler bei shmat(): shmid %d\n", shmid);

    while (TRUE){
        if(e==0){
          break;
        }
        e = 1;
printf("%s\n","anfang 1 while schleife" );
        fd = accept(sock, &client, &client_len);
printf("%s\n","nach accept" );
        int pid = fork();
        if(pid<0){ // nicht funktioniert
            printf("Speicher voll");
            return 1;
        }else if(pid>0){ // Vater
            continue;
        }else{ //pid == 0 --> Kindprozess
            write(fd, begruesung,strlen(begruesung));
printf("nach begr��ung %d\n", (int)strlen(begruesung) );
            while (e==1){
	bzero(input, 1024);
  bzero(key, 30);
  bzero(value, 30);
  //eingabe variablen leeren
  //memset(command,0,sizeof(command));
  //memset(key,0,sizeof(key));
  //memset(value,0,sizeof(value));

	            read(fd, input, 2000);
	            input[strlen(input)-2]='\0'; //

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
fflush(stdout); //erzwingt eine ausgabe

          //geht die command durch und vergleicht welche eingabe erfolgt ist
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
             //Verbidnung zum Client beenden
             close(fd);
             //Shared Memory entfernen
             shmdt(shar_mem);

             e = 0;
             printf("beendet\n");
             strcpy(res,"bey\n");
             write(fd, res,strlen(res));


           }else {printf("falsche eingabe\n");
                  strcpy(res,"falsche eingabe\n");
                  write(fd, res,strlen(res));
           }
         }//end while2
       }//end Kindprozess
    } //end while 1

//Shared Memory löschen
 result = shmctl(shmid, IPC_RMID, 0); // IPC_RMID löscht --> buffer = 0
 if (result == -1)
      printf ("Fehler bei shmctl() shmid %d, Kommando %d\n",
          shmid, IPC_RMID);
 return 0;
}
