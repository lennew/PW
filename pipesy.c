#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

int main () {
    int fd[2];
    char buf[32];
    int n;
    
    if (pipe(fd) == -1) {
        perror("pipe creation error");
        exit(1);
    }
    
    if (fork() == 0) {
        close(fd[0]);
        dup2(fd[1], 1);
        if (write(fd[1], "Hallo!\n", 7) == -1) {
            perror("child write error");
            exit(1);
        }
        close (fd[1]);
        exit(0);
    }
    
    if (fork() == 0) {
        close(fd[1]);
        dup2(fd[0],0);
        while((n = read(fd[0], buf, sizeof(buf))) > 0)
            write(1, buf, n);
        if (n == -1) {
            perror("child read error");
            exit(1);
        }
        close(fd[0]);
        exit(0);
    }
    close(fd[0]);
    close(fd[1]);
         
    if(wait(NULL) == -1) {
        perror("wait on writing child");
        exit(1);
    }
    if(wait(NULL) == -1) {
        perror("wait on writing child");
        exit(1);
    }
    exit(0);
}
