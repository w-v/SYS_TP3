#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/sem.h>

#define TAILLE 1024

int main (){
  int id2;
  struct sembuf up = {0,1,0};

  id2 = semget((key_t)6666,1,0600|IPC_CREAT);

  semop(id2,&up,1);
}
