#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/sem.h>


#define TAILLE 1024

void ecrire_tableau(int *compteur, char *tableau) {
  char message[64], *msg=message;
  snprintf(message, 64, "Je suis le processus %d!\n", getpid());

  while ((*compteur<TAILLE)&&(*msg)) {
    tableau[*compteur] = *msg;
    msg++;
    usleep(100000);
    (*compteur)++;
  }
}

//void initialize(){}
//mettre semctl dans nettoie.c
//

int main() {
  int id, *compteur;
  char *tableau;

  id = shmget((key_t)1234,TAILLE+sizeof(int),0600|IPC_CREAT);
  if (id<0) { perror("Error shmget"); exit(1); }

  int id2;
  struct sembuf down = {0,-1,0};
  struct sembuf up = {0,1,0};

  id2 = semget((key_t)6666,1,0600|IPC_CREAT);

  compteur = (int*) shmat(id,0,0);
  if (compteur==NULL) { perror("Error shmat"); exit(1); }

  tableau = (char *)(compteur + 1);
  
  semop(id2,&down,1);

  ecrire_tableau(compteur, tableau);

  semop(id2,&up,1);

  printf("%s\n", tableau);

  if (shmdt(compteur)<0) { perror("Error shmdt"); exit(1); }
  return 0;
}

