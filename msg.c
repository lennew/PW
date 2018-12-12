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
#include <sys/msg.h>
#include <time.h>

int msgid;
#define M_DANE 1
#define M_END 2
#define M_WYNIK 3

struct msgbuf {
    long mtype;
    char mint;
};


int main() {
    if((msgid = msgget(0x250, IPC_CREAT | 0777)) == -1) {
        perror("Cannot create msg");
        exit(1);
    }

    if (fork() == 0) {
        struct msgbuf sent = { 1, 1};
        while (sent.mint != 0) {
        
        if(msgsnd(msgid, &sent, sizeof(sent.mtext), 0) == -1) {
            perror("Cannot send the message");
            exit(1);
        }
        printf("%s", recieved.mtext);
        }
        struct msgbuf recieved;
        if(msgrcv(msgid, &recieved, BUF, 2, 0) == -1) {
            perror("Cannot send the message");
            exit(1);
        }
        exit(0);
    }
    if (fork() == 0) {
        struct msgbuf recieved;
        if(msgrcv(msgid, &recieved, BUF, 1, 0) == -1) {
            perror("Cannot recieve the message");
            exit(1);
        }

        char tmp[] = "World\n";
        strcat(tmp, recieved.mtext);
        struct msgbuf sent = { 2, ""};
        strcpy(sent.mtext, tmp);


        if(msgsnd(msgid, &sent, sizeof(sent.mtext), 0) == -1) {
            perror("Cannot send the message");
            exit(1);
        }
        exit(0);
    }
    wait(NULL);wait(NULL);
    if(msgctl(msgid, IPC_RMID, NULL)) {
        perror("Cannot remove msg");
        exit(1);
    }

}
