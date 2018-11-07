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
    int file = open("grupy.txt", O_WRONLY);
    
    if (pipe(fd) == -1) {
        perror("pipe creation error");
        exit(1);
    }
    
    if (fork() == 0) {
        close(fd[0]);
        dup2(fd[1], 1);
        execlp("cat", "cat", "/etc/group", NULL);
        close (fd[1]);
        exit(0);
    }
    
    if (fork() == 0) {
        close(fd[1]);
        dup2(fd[0],0);
        dup2(file, 1);
        execlp("head", "head", "-5", NULL);
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
