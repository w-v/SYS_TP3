#Exercise on semaphores with sys/sem.h

bin/tp3 writes into a string in shared memory  
Executing it twice within ~30 seconds doesn't scramble the writes thanks to a semaphore
The one executed second waits for the first one to finish his write to the string

## Notes

- src/initialize.c is no longer necessary, semaphore initialization is taken care of inside src/tp3.c
- bin/nettoie deletes the shared memory and semaphore used by the program
