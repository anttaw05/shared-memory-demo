# shared-memory-demo
Minimal OS concurrency simulator with shared memory and semaphores. This project was completed as a small university assignment for the SYSC 4001 Operating systems course to demonstrate fundamentals of process management, shared memory, and semaphores in a Linux environment.

# Features
Forking a child process with **fork()** \n
Child process executes a separate program via **exec()** (memory image replacement)
Inter-process communication using **shared memory** (shmget, shmat)
Mutual exclusion with **semaphores** (semget, semop)
Synchronization of parent/child process cycles
Clean termination and **memory cleanup**

# Environment / Reqirements
**Operating System:** Linux (Ubuntu recommended)
**Compiler:** g++
**Libraries:** Standard C++ Library (no external dependencies)
**Terminal:** For running the simulation

[!NOTE]
This project was tested in WSL, providing a Linux-compatible environment on Windows. Behavior may differ on different environments.

## Running the demo

1. **Compile the code:**
#run the build script :
```
source build.sh
```
3. **Run the parent program**
```
./main
```
