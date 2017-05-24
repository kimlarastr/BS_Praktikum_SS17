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
int delFlag[N];
int size = 0;

int put(char* key, char* value, char* res){

  for(int i=0; i<size; i++){
    if(strcmp(key,daten[i].key)==0){
      strcpy(res,daten[i].value);
      strcpy(daten[i].value,value);
      return 0;
    }
  }
  for(int j=0; j<size; j++){
    if(delFlag[j] == 1){
      strcpy(daten[j].key,key);
      strcpy(daten[j].value,value);
      return 0;
    }
  }
//if
	strcpy(daten[size].key,key);
      strcpy(daten[size].value,value);
    size++;
	return 0;
  //return printf("Kein Platz mehr");
}

int get(char* key, char* res){
  for(int i=0; i<size; i++){
    if(strcmp(daten[i].key,key)==0 && delFlag[i] != 1){
      strcpy(res,daten[i].value);
      return 0;
    }
  }
  return 1;
}

int del(char* key, char* res){
  for(int i=0; i<size; i++){
    if(strcmp(daten[i].key,key)==0){
      delFlag[i] = 1;
      return 0;
    }
   }
  return 1;
}
