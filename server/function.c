#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include "function.h"


//int delFlag[N]; //zum überprüfen, ob etwas drin steht oder der Platz frei ist
//int size = 0; //Anzahl eingefügter Elemente

int put(char* key, char* value, char* res, DATA* daten){
  /* prüfen, ob ein value eingegeben wurde
  if(strlen(value)=0){
    strcpy(res,"kein value eingegeben. Schlüssel nicht gespeichert");
    return 0;
  }
  */
  //key schon vorhanden und ersetzten
  for(int i=0; i<N; i++){
    if(strcmp(key,daten[i].key)==0){
      strcpy(res,daten[i].value);
      strcpy(daten[i].value,value);
      return 0;
    }
  }
  //schon wieder ein platz frei geworden?, wenn ja speicher es darein(wenn schon etwas gelöscht wurde)?
  for(int j=0; j<N; j++){
    if(daten[j].delFlag == 1){   // 1 = leerer Platz
      strcpy(daten[j].key,key);
      strcpy(daten[j].value,value);
      strcpy(res,"eingefügt\n");
      daten[j].delFlag = 0;
      return 0;
    }
  }
  /*
  if(size<N){
	  strcpy(daten[].key,key);
    strcpy(daten[].value,value);
    strcpy(res,"eingefügt\n");
    //size++;
	  return 0;
  }*/
  printf("Kein Platz mehr");
  return 1;

}

int get(char* key, char* res, DATA* daten){
  for(int i=0; i<N; i++){
    if(strcmp(daten[i].key,key)==0 && daten[i].delFlag != 1){ //es steht etwas drin, aber laut delFlag sollte nichts drin stehen
      strcpy(res,daten[i].value);
      return 0;
    }
  }
  strcpy(res,"key ist nicht enthalten\n");
  return printf("key %s nicht enthalten\n", key);
}

int del(char* key, char* res, DATA* daten){
  for(int i=0; i<N; i++){
    if(strcmp(daten[i].key,key)==0){
      daten[i].delFlag = 1;  // es wird nicht tatsächlich der speicher gelöscht, sondern nur die anzeige "platz frei"
      strcpy(res,daten[i].value);
      return 0;
    }
   }
  strcpy(res,"key ist nicht enthalten\n");
  return printf("key %s nicht enthalten\n", key);
}
