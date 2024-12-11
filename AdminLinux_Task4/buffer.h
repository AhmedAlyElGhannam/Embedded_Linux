#ifndef BUFFER_H
#define BUFFER_H

/** used libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>    // for key_t && ICP_CREAT flag
#include <sys/shm.h>    // for shared memory system calls like shmget && shmat
#include <sys/types.h>  // for key_t
#include <semaphore.h>  // for semaphores to avoid deadlocks/race conditions
#include <fcntl.h>      // for flags used in semaphore operations
#include <errno.h>      // for printing errors


/** defined macros */
// defining a unique key for the shared memory (will be the same for both processes)
#define SHM_KEY         6942

// defining max size for shared buffer
#define BUFFER_SIZE     256

// defining macros for true and false
#define TRUE    1
#define FALSE   0

// defining a struct for the data shared between processes (semaphores + buffer)
typedef struct 
{
    sem_t bin_sem;                // only one process can hold it
    sem_t dataAvailable;       // to signal data availability
    sem_t spaceAvailable;      // to signal space availability
    char buf[BUFFER_SIZE];   // for inter-process data transfer operations 
} sharedBuffer_t;

#endif
