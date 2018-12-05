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
#include <sys/sem.h>
#include <time.h>

int semid;
static struct sembuf buf;

void podnies(int semid, int semnum) {
    buf.sem_num = semnum;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    
    if(semop(semid, &buf, 1) == -1) {
        perror("podnoszenie semafora");
        exit(1);
    }
}

void opusc (int semid, int semnum) {
    buf.sem_num = semnum;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    
    if (semop(semid, &buf, 1) == -1) {
        perror("opuszczanie semafora");
        exit(1);
    }
}

void handler () {
    if (semctl(semid, 0, IPC_RMID, 0) == -1) {
        perror("cannot remove semaphore");
        exit(1);
    }
    exit(0);
}

int tworzFilozofa (int numer) {
    if (fork() == 0) {
        write (1, numer);
        perror ("Cannot create child")
        exit(1);
    }
    opusc(semid, numer - 1);
    printf("Filozof:%d: Wzialem widelec lewy\n", numer);
    opusc(semid, numer);
    printf("Filozof:%d: Wzialem widelec prawy\n", numer);
    sleep(rand() % 4);
    printf("Filozof:%d: Zjadlem\n");
    podnies(semid, numer);
    printf("Filozof:%d: Polozylem widelec prawy\n", numer);
    opusc(semid, numer);
    printf("Filozof:%d: Polozylem widelec lewy\n", numer);
}


int main() {  
    if ((semid = semget(0x112, 1, IPC_CREAT | 0777)) == -1) {
        perror("Cannot get semaphore");
        exit(1);
    }
    if (semctl(semid, 0, SETVAL, 1) == -1) {
        perror("Cannot set semaphore's value");
        exit(1);
    }
    
    srand(time(NULL));
    exit(1);
}
