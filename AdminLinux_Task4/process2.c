#include "buffer.h"

int main(void) 
{
    // creating shared memory using shmget => takes key, size of data struct, Flag to ensure memory creation ORed with 0666 to ensure all users' processes from all groups can read and write in it
    int shmid = shmget(SHM_KEY, sizeof(sharedBuffer_t), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // attaching data to shared memory with the id created above => takes shared memory id, NULL to indicate that the OS will decide the data's address, 0 to have read and write access.
    sharedBuffer_t *shared = (sharedBuffer_t *)shmat(shmid, NULL, 0);
    if ((int)shared == -1) // ensure shared memory was attached successfully
    {
        // print error message indicating that the cause is shmat and exit
        perror("process2: syscall shmat returned -1: could not attach data to shared memory.");
        exit(1);
    }

    // defining input buffer
    char out_buf[BUFFER_SIZE];

    while (TRUE) 
    {
        // wait until there is new data available in order to read it from shared memory buffer
        sem_wait(&shared->dataAvailable); 

        /**CRITICAL SECTION START*/

        // wait until process1 is no longer is using the shared memory buffer
        sem_wait(&shared->bin_sem);          

        // copy message from shared memory buffer into output buffer
        strncpy(out_buf, shared->buf, BUFFER_SIZE);

        // print the content of the output message taken from shared memory buffer
        printf("Process2 Read %s from Shared Memory.\n", out_buf);

        // release ownership of the shared memory by freeing the binary semaphore
        sem_post(&shared->bin_sem);
        
        /**CRITICAL SECTION END*/

        // signal to process1 that data have been read from shared memory buffer and it is free to send again
        sem_post(&shared->spaceAvailable); 

        // if the received string is "exit" => end program
        if (strncmp(out_buf, "exit", 4) == 0)
            break;
    }

    // detach shared memory before exiting
    shmdt(shared);

    // free shared memory upon exiting
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
