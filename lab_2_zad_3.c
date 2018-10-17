#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const int MAX = 512;

int endOfLine(char buf[], int n){
    for (int i = 0; i < n; ++i){
        if (buf[i] == '\n') return i;
    }   
    return -1;
}

int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("Zła ilość argumentów");
        exit(1);
    }
    
    int fd = open (argv[1], O_RDWR);
    if (fd == -1) {
        perror("Opening file error!");
    }
    
    
    int n, newLine, max = 0;
    char buf[MAX];
    while ((n = read(fd, buf, MAX)) > 0) {
        newLine = endOfLine(buf, n);
 	if (newLine > max) max = newLine;
	lseek(fd, -n + newLine + 1, SEEK_CUR);
    }
    printf("%d\n", max);
    if (n == -1) {
        perror("Reading error");
        exit(1);
    }
    
    close(fd);
    
    return 0;
}