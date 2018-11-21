#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

int flag;

void handler() {
    flag = 1 - flag;
}

int main() {
    flag = 0;
    signal(SIGINT, handler);
    while(1) {
        sleep(3);
        if(fork() == 0) {
            if(flag == 0) execlp("ls", "ls", NULL);
            else execlp("ps", "ps", NULL);
        }
    }
}
