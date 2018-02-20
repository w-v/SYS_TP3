#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/sem.h>
#include <errno.h>



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

int main() {
  int id, *compteur;
  char *tableau;

  // creates shared memory with key 1234 if it doesn't exist, gets its id
  id = shmget((key_t)1234,TAILLE+sizeof(int),0600|IPC_CREAT);
  if (id<0) { perror("Error shmget"); exit(1); }

  int id2;
  struct sembuf down = {0,-1,0};
  struct sembuf up = {0,1,0};
  
  /* inializing the semaphore */

  // adding IPC_EXCL to permisisons causes an error if semaphore 6666 already exists
  id2 = semget((key_t)6666,1,0600|IPC_CREAT|IPC_EXCL);
  
  if (id2<0) { 

    if(errno != EEXIST) {
      // error was not about the semaphore having already been created
      fprintf(stderr,"Error semget errno: %d\n",errno);
      exit(1);
    
    }
    else {
      // semaphore 6666 already exists, getting its id
      // (this time without having to worry about wether it already exists)
      // (without IPC_EXCL)
      id2 = semget((key_t)6666,1,0600|IPC_CREAT);
    
    }

  }
  else {

    // semget did not throw an error, semaphore 6666 didn't already exist
    // this process just created it
    
    // initializing it to 1
    semop(id2,&up,1);

  }

  /* using the semaphore */

  // attaches this process to the shared memory, gets its starting address 
  compteur = (int*) shmat(id,0,0);
  if (compteur==NULL) { perror("Error shmat"); exit(1); }

  tableau = (char *)(compteur + 1);
  
  semop(id2,&down,1);                   // DOWN()

  ecrire_tableau(compteur, tableau);    // critical section
                                        // writing into shared memory
                                        // at a shared address (compteur)

  semop(id2,&up,1);                     // UP()

  printf("%s\n", tableau);

  // detaches shared memory at address compteur from this process
  if (shmdt(compteur)<0) { perror("Error shmdt"); exit(1); }
  
  return 0;
}

