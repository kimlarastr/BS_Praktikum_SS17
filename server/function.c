#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


int n = 1024;

struct data{
  char* key;
  char* value;
};

struct data daten[n];

int put(char* key, char* value, char* res){

  for(int i=0; i<n; i++){
    if(*key == daten[i].key){
      *res = daten[i].value;
      //daten[i].key = *key;
      daten[i].value = *value;
      return 0;
    }
  }
  for(int j=0; j<sizeof(daten); j++){
    if(daten[j]==NULL){
      daten[j].key = *key;
      daten[j].value = *value;
      return 0;
    }
  }
  return -1;
}
//aufruf put(&key1, &value, &res)
int get(char* key, char* res){
  for(int i=0; i<sizeof(daten); i++){
    if(daten[i].key == *key){
      res = daten[i].value;
      return 0;
    }
  }
  return -1;
}

int del(char* key, char* res){
  for(int i=0; i<sizeof(daten); i++){
    if(daten[i].key == *key){
      res = daten[i].value;
      daten[i].key = NULL;
      daten[i].value = NULL;
      return 0;
    }
   }
  return -1;
}

int main(int argc, char*argv[]) // Portnr. & Ip adresse
{
  
char input[5];
char key[30];
char value[30];

printf( "Verwenden sie bitte eine der Funktionen PUT(key value), GET(key); DEL(key) oder beenden sie die Kommunikation mit EXIT" );
scanf( "%s %s %s", &input, &key, &value);
switch ( input ) {
   case "PUT":
       put();
       break;
   case "GET":
       if()
       get();
       break;
   case "DEL":
       del();
       break;
   case "EXIT":
       printf( "Thanks for using. \n" );
       break;
   default:
       printf( "Bad input!\n" );
       break;
}
*/
