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
        perror("process1: syscall shmat returned -1: could not attach data to shared memory.");
        exit(1);
    }

    // initializing semaphores => takes the address of the semaphore, 1 to indicate that this semaphore is shared between processes, and a value
    sem_init(&shared->bin_sem, 1, 1);           // binary semaphore so its value is 1
    sem_init(&shared->dataAvailable, 1, 0);     // counting semaphore initialized to 0
    sem_init(&shared->spaceAvailable, 1, 1);    // counting semaphore initialized to 1

    // defining input buffer
    char in_buf[BUFFER_SIZE];

    // keep this process alive and keep reading messages into buffer
    while (TRUE) 
    {
        // read message from user into process1
        printf("Enter a Message to Send || Type \"exit\" to Quit: \n");
        
        // store input message into input buffer
        fgets(in_buf, BUFFER_SIZE, stdin);

        // wait until there is available space in order to write into shared memory buffer
        sem_wait(&shared->spaceAvailable);

        /**CRITICAL SECTION START*/

        // wait until process2 is no longer using the shared memory buffer
        sem_wait(&shared->bin_sem);  

        // copy input message from input buffer into shared memory 
        strncpy(shared->buf, in_buf, BUFFER_SIZE);

        // print the content of input buffer for reference
        printf("Process1 Wrote %s in Shared Memory.\n", in_buf);

        // release ownership of the shared memory by freeing the binary semaphore
        sem_post(&shared->bin_sem);         

        /**CRITICAL SECTION END*/

        // signal to process 2 that new data have been written into shared memory and it is free to read again
        sem_post(&shared->dataAvailable); 

        // if the user entered "exit" => end program
        if (strncmp(in_buf, "exit", 4) == 0)
            break;
    }

    // detach shared memory before exiting
    shmdt(shared);

    return 0;
}
