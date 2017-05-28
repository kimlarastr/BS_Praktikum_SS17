#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include "function.h"

struct data{
  char key[30];
  char value[30];
};

struct data daten[N];
int delFlag[N]; //zum überprüfen, ob etwas drin steht oder der Platz frei ist
int size = 0; //Anzahl eingefügter Elemente

int put(char* key, char* value, char* res){
  /* prüfen, ob ein value eingegeben wurde
  if(strlen(value)=NULL){
    strcpy(res,"kein value eingegeben. Schlüssel nicht gespeichert");
    return 0;
  }
  */
  //key schon vorhanden und ersetzten
  for(int i=0; i<size; i++){
    if(strcmp(key,daten[i].key)==0){
      strcpy(res,daten[i].value);
      strcpy(daten[i].value,value);
      return 0;
    }
  }
  //schon wieder ein platz frei geworden?, wenn ja speicher es darein(wenn schon etwas gelöscht wurde)?
  for(int j=0; j<size; j++){
    if(delFlag[j] == 1){   // 1 = leerer Platz
      strcpy(daten[j].key,key);
      strcpy(daten[j].value,value);
      strcpy(res,"eingefügt\n");
      delFlag[j] = 0;
      return 0;
    }
  }
  if(size<N){
	  strcpy(daten[size].key,key);
    strcpy(daten[size].value,value);
    strcpy(res,"eingefügt\n");
    size++;
	  return 0;
  }
  return printf("Kein Platz mehr");
}

int get(char* key, char* res){
  for(int i=0; i<size; i++){
    if(strcmp(daten[i].key,key)==0 && delFlag[i] != 1){ //es steht etwas drin, aber laut delFlag sollte nichts drin stehen
      strcpy(res,daten[i].value);
      return 0;
    }
  }
  strcpy(res,"key ist nicht enthalten\n");
  return printf("key %s nicht enthalten\n", key);
}

int del(char* key, char* res){
  for(int i=0; i<size; i++){
    if(strcmp(daten[i].key,key)==0){
      delFlag[i] = 1;  // es wird nicht tatsächlich der speicher gelöscht, sondern nur die anzeige "platz frei"
      strcpy(res,daten[i].value);
      return 0;
    }
   }
  strcpy(res,"key ist nicht enthalten\n");
  return printf("key %s nicht enthalten\n", key);
}
