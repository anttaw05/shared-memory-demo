#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

struct Shared_data {
    int multiple;
    int counter;
};

// Semaphore operations
struct sembuf sem_lock = {0, -1, 0};  // P operation
struct sembuf sem_unlock = {0, 1, 0}; // V operation

int main() {
    struct Shared_data* data; 
    void* shared_memory = (void*)0;

    // Create shared memory
    int shmid = shmget((key_t)1234, sizeof(struct Shared_data), 0666 | IPC_CREAT);
    if (shmid == -1) { perror("shmget failed"); exit(1); }

    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void*)-1) { perror("shmat failed"); exit(1); }

    data = (struct Shared_data*) shared_memory;
    printf("Memory attached to parent.\n");

    // Initialize shared data
    data->multiple = 3;
    data->counter = 0;

    // Create semaphore
    int semid = semget((key_t)5678, 1, 0666 | IPC_CREAT);
    if (semid == -1){
        perror("semget failed");
        exit(1);
    }

    // Initialize semaphore to 1
    semctl(semid, 0, SETVAL, 1);

    printf("fork program starting\n");
    pid_t pid = fork();
    if (pid == -1){ 
        perror("fork failed"); 
        exit(1);
    }
    if (pid == 0) {
        execl("./childP5", "./childP5", NULL);
        perror("execl failed");
        exit(1);
    }

    // Parent loop
    while (1) {
        semop(semid, &sem_lock, 1); // Lock semaphore
        if (data->counter >= 300) {
            semop(semid, &sem_unlock, 1);
            break;
        }

        if (data->counter % data->multiple == 0)
            printf("Parent process Cycle: %d\n", data->counter);

        data->counter++;
        semop(semid, &sem_unlock, 1); // Unlock semaphore

        usleep(50000); // speed: 5x faster
    }

    // Cleanup
    printf("Parent & Child process Cycle has reached 500, Execution Terminated!\n");
    printf("Wiping shared memory.\n");
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID, 0);

    wait(NULL); // wait for child
    return 0;
}
