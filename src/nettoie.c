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
	int id = shmget((key_t)1234,TAILLE+sizeof(int),0600|IPC_CREAT);
	shmctl(id, IPC_RMID, NULL);
	int id_sem = semget((key_t)6666,1,0600|IPC_CREAT);
	semctl(id_sem,0,IPC_RMID);
}
