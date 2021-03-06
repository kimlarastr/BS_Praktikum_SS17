#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/sem.h>  // Headerfile für Semaphore
#include <unistd.h>
#include<signal.h> //signalHandler für tastendruck abzufangen
#include <string.h>
#include "function.h"

//static bool keepRunning = true;
int shmid;   // Shared Memory id
int sem_id;
int result;
int fd;
char input[1024];
char command[10]; //PUT GET DEL oder EXIT
char key[30];
char value[30];
char res[2000];
char* begruesung = "Verwenden Sie bitte eine der Funktionen PUT(key value), GET(key); DEL(key) oder beenden sie die Kommunikation mit EXIT\n";
char* teil; // um den input zu trennen
int e=1;  // um die innere while zu beenden --> momentan nicht gebraucht
int pid;
DATA *daten; // Shared Memory
int readerid;
int *reader = 0; // Variable wie viele Leser es gibt --> für semaphore

int readDatei(struct DATA *daten){
  FILE *ptr;
  ptr = fopen("konsistenteDaten.bin","rb");  // r for read, b for binary
  if(ptr != NULL){
      fread(daten,sizeof(DATA)*N,1,ptr); // read 10 bytes to our buffer
      fclose(ptr);
  }
  return 0;
}

int writeDatei(struct DATA *daten){
  FILE *write_ptr;
  write_ptr = fopen("konsistenteDaten.bin","wb");  // w for write, b for binary
  if(write_ptr != NULL){
      fwrite(daten, sizeof(DATA) * N,1,write_ptr); // write 10 bytes from our buffer
      fclose(write_ptr);
  }
  return 0;
}

void intHandler(int sig) {
  //keepRunning = false
  printf("\nkilling process %d\n",getpid());
  //Daten speichern bevor Server geschlossen wird
  writeDatei(daten);
  //Shared Memory löschen
   result=shmctl(shmid, IPC_RMID, NULL); // IPC_RMID löscht --> buffer = 0
   if (result == -1){
        printf ("Fehler bei shmctl() shmid %d, Kommando %d\n",shmid, IPC_RMID);
   }
   //Shared Memory 2 löschen
    result=shmctl(readerid, IPC_RMID, NULL); // IPC_RMID löscht --> buffer = 0
    if (result == -1){
         printf ("Fehler bei shmctl() shmid %d, Kommando %d\n",readerid, IPC_RMID);
    }
   // Semaphor löschen
   semctl(sem_id, 1, IPC_RMID, 0);
   /* auslesen der Daten
   for(int i = 0; i<10; i++)
    printf("%u ", buffer[i]); // prints a series of bytes
   */
   printf("Daten gespeichert. Server geschlossen");
   fflush(0);
   exit(0);
}

int main(int argc, char*argv[]){

  signal(SIGINT, intHandler);
  signal(SIGTERM, intHandler);

// Variable sem_id für die Semaphorengruppe und
// aus technischen Gründen eine Variable marker[1].

 unsigned short marker[1];
  // Semaphorengruppe(nur ein Semaphor erzeugt)
   sem_id = semget (IPC_PRIVATE, 1, IPC_CREAT|0644);
   if (sem_id == -1) {
      perror ("Die Gruppe konnte nicht angelegt werden!");
      exit(1); }

  // Anschließend wird der Semaphor auf 1 gesetzt
  marker[0] = 1;
  semctl(sem_id, 1, SETALL, marker);  // alle Semaphoren auf 1

struct sembuf {
  short sem_num; //sem_numist dabei die Nummer des Semaphors innerhalb der Gruppe
   short sem_op; //Wert sem_oplegt die auszuführende Operation fest:
                 //Wert größer 0(UP-Operation), Wert kleiner 0(DOWN-Operation)
   short sem_flg; //Mit sem_flg können Flags zur Steuerung angegeben werden
  };

struct sembuf enter;
struct sembuf leave;
struct sembuf readUp;
struct sembuf readDown; // Structs für den Semaphor
 enter.sem_num = leave.sem_num = 0;// Semaphor 0 in der Gruppe
 enter.sem_flg = leave.sem_flg = SEM_UNDO; // SEM_UNDO bei unabsichtlichen schließen zurück gesetzt
 enter.sem_op = -1; // blockieren, DOWN-Operation
 leave.sem_op = 1;// freigeben, UP-Operation

 readUp.sem_num = readDown.sem_num = 0;
 readUp.sem_flg = readDown.sem_flg = SEM_UNDO;
 readUp.sem_op = 1;
 readDown.sem_op = -1;

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

printf("%s\n","socket erstellt" );
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

    listen(sock,5);

    struct sockaddr client;
          socklen_t client_len;
	  client_len= sizeof(client);

    //erzeugen und anlegen des Shared Memory
    shmid = shmget (IPC_PRIVATE, sizeof(DATA)*N, IPC_CREAT | 0777 );
    if (shmid == -1){
       printf ("Fehler bei key %d, mit der Größe %d\n",IPC_PRIVATE, N);
    }
    //erzeugen und anlegen des 2 Shared Memory für Semaphor
    readerid = shmget (IPC_PRIVATE, sizeof(int), IPC_CREAT | 0777 );
    if (readerid == -1){
       printf ("Fehler bei key %d\n",IPC_PRIVATE);
    }

    //anhängen/binden des Shared Memory
    daten = (struct data*) shmat(shmid, 0, 0);
    if (daten == (void *) -1){
      printf ("Fehler bei shmat(): shmid %d\n", shmid);
      exit(1);
    }
    //anhängen/binden des 2 Shared Memory
    reader = (int*) shmat(readerid, 0, 0);
    if (reader == (void *) -1){
      printf ("Fehler bei shmat(): readerid %d\n", readerid);
      exit(1);
    }

    //delFlag auf 1 setzten an jedem Speicherplatz
    for(int i=0; i<N; i++){
      daten[i].delFlag=1;
    }
    //auslesen der Daten und speichern in den Shared Memory --> Funktion readDatei
    readDatei(daten);



    while (TRUE){

        if(e==0){
          printf("ende von Kindprozess\n");
          //for(int i=0; i<NumberOfChild; i++){
          //waitpid(pid[i], NULL, 0);
        //  }
          break;
        }
        //e = 1;
printf("%s\n","anfang 1 while schleife" );
        fd = accept(sock, &client, &client_len);
printf("%s\n","nach accept" );

        pid = fork();
        if(pid<0){ // nicht funktioniert
            printf("fork() fehlgeschlagen"); //speicher voll
            return 1;
        }else if(pid>0){ // Vater
		printf("%d\n", pid);
            continue;
        }else{ //pid == 0 --> Kindprozess
             printf("Kindprozess %d gestartet \n", pid);
            write(fd, begruesung,strlen(begruesung));
printf("nach begr��ung %d\n", (int)strlen(begruesung) );
            while (e==1){
printf("%s\n","anfang 2 while schleife" );
printf("Kindprozess %d gestartet \n", pid);
  //eingabe variablen leeren --> bzero in .h überschrieben
	             bzero(input, 1024);
               bzero(key, 30);
               bzero(value, 30);
  //memset(command,0,sizeof(command));
  //memset(key,0,sizeof(key));
  //memset(value,0,sizeof(value));

	            read(fd, input, 2000);
	            input[strlen(input)-2]='\0'; // mitgesendeter tastendruck von enter(/N/R) rauslöschen

              //input teilen in command, key, value
              teil = strtok(input, " ");
//printf("%s%d\n", teil, strlen(teil));
              strcpy(command,teil);
//printf("%s%d\n", command, strlen(command));
              teil = strtok(NULL," ");
//printf("%s%d\n", teil, strlen(teil));
              strcpy(key,teil);
//printf("%s%d\n", key, strlen(key));
              teil = strtok(NULL, '\0');
//printf("%s%d\n", teil, strlen(teil));
              teil = strcpy(value,teil);
//printf("%s%d\n", value, strlen(value));
//printf("Alle teile eingelesen");
fflush(stdout); //erzwingt eine ausgabe

          //geht die command durch und vergleicht welche eingabe erfolgt ist
           if(strcmp(command,"PUT")==0){
             semop (sem_id, &readDown, 1);// Eintritt in kritischen Bereich
             put(key, value, res, daten);
             semop (sem_id, &readUp, 1); // Verlassen des kritischen Bereich
             printf("%s\n",res);
        //  sleep(20);
             write(fd, res,strlen(res));

          }else if(strcmp(command, "GET")==0){
             semop (sem_id, &enter, 1);// Eintritt in kritischen Bereich
             *reader = *reader +1; //Ein Leser mehr
             if((int)reader == 1){
               semop (sem_id, &readDown, 1);
             }
             semop (sem_id, &leave, 1); // Verlassen des kritischen Bereich
             printf("Leser anzahl: %d \n", *reader );
     //sleep(5);
             get(key, res, daten);

             semop (sem_id, &enter, 1);// Eintritt in kritischen Bereich
             *reader = *reader -1; //Ein Leser weniger
             if(reader == 0){
               semop (sem_id, &readUp, 1);
             }
             semop (sem_id, &leave, 1); // Verlassen des kritischen Bereich
             printf("%s\n",res );
             strcat(res,"\n"); //fügt zeilenumbruch bei
             write(fd, res,strlen(res));


           }else if(strcmp(command, "DEL")==0){
             semop (sem_id, &readDown, 1);// Eintritt in kritischen Bereich
             del(key, res, daten);
             semop (sem_id, &readUp, 1); // Verlassen des kritischen Bereich

             printf("%s\n",res);
             strcat(res, "\n");
             write(fd, res,strlen(res));

           }else if(strcmp(command, "EXIT")==0){
             //Verbidnung zum Client beenden
             printf("Kindprozess %d gestartet \n", getpid());
             strcpy(res,"bye\n");
             write(fd, res,strlen(res));
             close(fd);
             //listen sock schließen
             close(sock);
             //Shared Memory entfernen
             shmdt(daten);
             e = 0;
             printf("beendet\n");
             exit(0);

           }else {printf("falsche eingabe\n");
                  strcpy(res,"falsche eingabe\n");
                  write(fd, res,strlen(res));
           }
         }//end while2
       }//end Kindprozess

                                    /*for(int i=0; i<NumberOfChild; i++){
                                    waitpid(pid[i], NULL, 0);
                                   }*/

    } //end while 1
 return 0;
}
