#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

const int BUF_SIZE = 1024;

int main() {
    char fifo[] = "ser_fifo";
    int n, fd;
    
    if (mkfifo(fifo, S_IRUSR | S_IWUSR) == -1) {
        perror("making fifo error");
        exit(1);
    }
    char buf[BUF_SIZE];
    
    while(1) {
        if ((fd = open(fifo, O_RDONLY)) == -1) {
            perror("reading error");
            exit(1);
        }
        read(fd, buf, 32);
        if (fork() == 0) {
            execlp(buf, buf, NULL);
            exit(0);
        }
    }
    unlink(fifo);
    return 0;
}
