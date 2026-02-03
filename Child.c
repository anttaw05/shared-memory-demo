#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

struct Shared_data {
    int multiple;
    int counter;
};

struct sembuf sem_lock = {0, -1, 0};  // P operation
struct sembuf sem_unlock = {0, 1, 0}; // V operation

int main() {
    struct Shared_data* data; 
    void* shared_memory = (void*)0;

    // Access shared memory
    int shmid = shmget((key_t)1234, sizeof(struct Shared_data), 0666);
    if (shmid == -1){ 
        perror("shmget failed");
        exit(1);
    }

    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void*)-1) {
        perror("shmat failed");
        exit(1);
    }

    data = (struct Shared_data*) shared_memory;

    // Access semaphore
    int semid = semget((key_t)5678, 1, 0666);
    if (semid == -1){
        perror("semget failed");
        exit(1);
    }

    int last_printed = -1;
    printf("Memory attached to child.\n");

    while (1) {
        semop(semid, &sem_lock, 1); // Lock semaphore

        if (data->counter == 100 ) {
            printf("Cycle 100 : Child process Cycle Beginning!\n");
            last_printed = data->counter;
        }

        if (data->counter > 100 && data->counter % data->multiple == 0 && data->counter != last_printed) {
            printf("Child process Cycle: %d\n", data->counter);
            last_printed = data->counter;
        }
        if(data->counter >= 300){
            semop(semid, &sem_unlock, 1); //Unlock semaphore
            break;
        }
        

        semop(semid, &sem_unlock, 1); // Unlock semaphore
        usleep(50000);
    }

    shmdt(shared_memory);
    return 0;
}
