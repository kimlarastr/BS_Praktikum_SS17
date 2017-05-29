
#define MAX 20
#define N 1024

struct data{
  char key[30];
  char value[30];
};

struct data daten[N];

int put(char* key, char* value, char* res);

int get(char* key, char* res);

int del(char* key, char* res);
