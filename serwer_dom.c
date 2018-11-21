#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>


//  dzieli ciąg znaków otrzymany od klienta
//  na pojedyńcze stringi
//  i wydziela jako pierwszy string nazwę fifo klienta
void getStr(char line[], char *strings[], char **clientsFifo) {
    
    *clientsFifo = line;
    while (*line != '#') ++line;
    *line = '\0';
    
    strings[0] = ++line;
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

const int BUF_SIZE = 1024;

int main() {
    char fifo[] = "ser_fifo";
    char *commands[BUF_SIZE];
    char *clientsFifo;
    int ser_fd, cli_fd, n;
    
    if (mkfifo(fifo, S_IRUSR | S_IWUSR) == -1) {
        perror("making fifo error");
        exit(1);
    }
    
    
    char buf[BUF_SIZE];
    
    while(1) {
        printf("serwer starts\n");
        
        //  ***CZYTAJ Z FIFO SERWERA***
        if ((ser_fd = open(fifo, O_RDONLY)) == -1) {
            perror("reading error");
            exit(1);
        }
        read(ser_fd, buf, BUF_SIZE);
        close(ser_fd);
        //  ***KONIEC CZYTANIA***
        
        getStr(buf, commands, &clientsFifo);
        //  ***PISZ DO FIFO KLIENTA***
        
        if (fork() == 0) {
            if ((cli_fd = open(clientsFifo, O_WRONLY)) == -1) {
                perror("opening clients fifo by server");
                exit(1);
            
            }
            dup2(cli_fd, 1);
            if (execvp(commands[0], commands) == -1);
            {
                printf("bad comm");
                char error[] = "Wrong command";
                strcpy(buf, error);
                exit(1);
            }
            exit(0);
        }
        if (wait(NULL) == -1) {
            perror("wait on child");
            exit(1);
        }
        
        //  ***KONIEC PISANIA***
    }
    return 0;
}
