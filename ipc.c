#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <string.h>

int shmid;

void handler () {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("cannot remove shared memory");
        exit(1);
    }
}

int main() {    
    char *buffer;
    char first[] = "Child1: Luke, use the power!\n";
    char second[] = "Child2: I am your father!\n";
    signal(SIGINT, handler);

    if ((shmid = shmget(0x000000666, sizeof(buffer), IPC_CREAT | 0777)) == -1) {
        perror("Cannot create shm");
        exit(1);
    }
    
    if ((buffer = shmat(shmid, NULL, 0)) == (void *) -1) {
            perror("Parent: Cannot attach shm");
            exit(1);
    }
    
    
    if (fork() == 0) {
        if ((buffer = shmat(shmid, NULL, 0)) == (void *) -1) {
            perror("Child1: Cannot attach shm");
            exit(1);
        }
        while(1)
            strcpy(buffer, first);
    }
    if (fork() == 0) {
        if ((buffer = shmat(shmid, NULL, 0)) == (void *) -1) {
            perror("Child2: Cannot attach shm");
            exit(1);
        }
        while(1)
            strcpy(buffer, second);
    }
    while(1) {
        sleep(1);
        printf("%s\n", buffer);
    }
    exit(1);
}
