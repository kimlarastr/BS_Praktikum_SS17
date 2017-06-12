
#define MAX 20
#define N 1024
#define NumberOfChild 5
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

typedef struct data{
  char key[30];
  char value[30];
  int delFlag;
} DATA;

//struct data* daten;

int put(char* key, char* value, char* res, DATA* daten);

int get(char* key, char* res, DATA* daten);

int del(char* key, char* res, DATA* daten);
