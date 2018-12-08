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
const int liczbaFilozofow = 5;
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
    if (semctl(semid, 0, IPC_RMID, NULL) == -1) {
        perror("cannot remove semaphore");
        exit(1);
    }
    exit(0);
}

void posadzFilozofa (int numer) {
    if (fork() == 0) {
        int mniejszy = numer;
        int wiekszy = numer + 1;
        
        char sMniejszy[6];
        strcpy(sMniejszy, "lewy");
        char sWiekszy[6];
        strcpy(sWiekszy, "prawy");
        
        while(1) {
            if (numer + 1 == liczbaFilozofow) {
                mniejszy = 0;
                wiekszy = numer;
                strcpy(sMniejszy, "prawy");
                strcpy(sWiekszy, "lewy");
            }
            opusc(semid, wiekszy);
            printf("Filozof %d: Wzialem widelec %s\n", numer, sWiekszy);
            opusc(semid, mniejszy);
            printf("Filozof %d: Wzialem widelec %s\n", numer, sMniejszy);
            sleep(rand() % 3);
            printf("Filozof %d: Zjadlem\n", numer);
            podnies(semid, mniejszy);
            printf("Filozof %d: Polozylem widelec %s\n", numer, sMniejszy);
            podnies(semid, wiekszy);
            printf("Filozof %d: Polozylem widelec %s\n", numer, sWiekszy);
        }
    }
}

void nakryjDoStolu () {
    if ((semid = semget(0x997, liczbaFilozofow, IPC_CREAT | 0777)) == -1) {
        perror("Cannot get semaphore");
        exit(1);
    }
    
    for (int i = 0; i < liczbaFilozofow; ++i) {
        if (semctl(semid, i, SETVAL, 1) == -1) {
            perror("Cannot set semaphore's value");
            exit(1);
        }
    }
}



int main() {  
    srand(time(NULL));
    
    nakryjDoStolu();
    for (int i = 0; i < liczbaFilozofow ; ++i) posadzFilozofa(i);
    
    signal(SIGINT, handler);
    while(1);
    exit(0);
}
