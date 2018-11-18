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

const int BUF_SIZE = 1024;

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        printf("Nieprawidlowa ilosc argumentow!\nPodaj nazwe kolejki FIFO klienta\n");
        exit(1);
    }
    char serFifo[] = "ser_fifo";
    char myFifo[sizeof(argv[1])];
    strcpy(myFifo, argv[1]);
    int n, ser_fd, my_fd;

    if (mkfifo(myFifo, S_IRUSR | S_IWUSR) == -1) {
        perror("Client_1 fifo making error");
        exit(1);
    }
    
    char buf[BUF_SIZE];
    char tmp[BUF_SIZE];
    
    while(1){
        printf("Klient starts\n");
        strcpy(buf, myFifo);
        strcat(buf, "#");
        fgets(tmp, BUF_SIZE, stdin);
        strcat(buf, tmp);
        
        //  ***PISZ DO FIFO SERWERA***
        if ((ser_fd = open(serFifo, O_WRONLY)) == -1) {
            perror("server fifo opening error (client)");
            exit(1);
        }
        
        write(ser_fd, buf, strlen(buf));
        close(ser_fd);
        //  ***KONIEC PISANIA***
        
        //  ***CZYTAJ Z FIFO KLIENTA***
        if ((my_fd = open(myFifo, O_RDONLY)) == -1) {
            perror("Client_1 fifo opening error");
            exit(1);
        }
        
        while((n = read(my_fd, buf, sizeof(buf))) > 0) {
            write(1, buf, n);
            printf("W petli read\n");
        }
        
        close(my_fd);
        printf("skocznylem czytac\n");
        //  ***KONIEC CZYTANIA***
    }
    exit(0);
}
