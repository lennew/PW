#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LINE 100
#define MAX_ARG 10

int getStr(char line[], char *strings[]) {
    strings[0] = line;
    int i  = 1;
    while(*line != '\0') {
        if(*line == ' ') {
            *line = '\0';
            strings[i] = ++line;
            ++i;
        } else {
            line++;
        }
    }
    *(--line) = '\0';
    strings[i] = NULL;
}

int main(){
    char line[MAX_LINE];
    char *strings[MAX_LINE];
    while(1){
        printf("basz> ");
        fgets(line, MAX_LINE, stdin);
        getStr(line, strings);
    
        if(strncmp(line, "exit", 4) == 0) break;
        
        int status, pid;
        if((pid == fork()) != 0) {
            wait(&status);
        }   else {
            execlp(strings[0], strings[1]);
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
    
}
