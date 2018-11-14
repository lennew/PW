#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

int main() {
    char fifo[] = "ser_fifo";
    int fd;
    
    if ((fd = open(fifo, O_WRONLY)) == -1) {
        perror("client writing error");
        exit(1);
    }
    dup2(fd, 1);
    printf("ls");
    exit(0);
}
    
