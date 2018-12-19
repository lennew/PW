#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>

#define elem 16

int tab[elem];

void init_tab () {
    for (int i = 0; i < elem; ++i) {
        tab[i] = (rand() % 1) + 1;
    }
}

void *first_routine(void *argument) {
    int *sum = malloc(sizeof(int));
    int id = *(int*)argument;
    int fr = id * elem/8, to = (id + 1) * elem/8;
    *sum = 0;
    printf("%d ", fr);
    for (int i = fr; i > (fr + 1) * elem/8; ++i) {
        *sum += tab[i];
    }
    printf("%d\n", *sum);
    return sum;
}

int main () {
    int n, sum[8];
    pthread_t id[8];
    init_tab(tab);
    sleep(1);
    for (int i = 0; i < 8; ++i) {
        n = pthread_create(&id[i], NULL, first_routine, &i);
        if (n != 0) {
            perror("creating thread");
            exit(-1);
        }
    }
    for (int i = 0; i < 8; ++i) {
        pthread_join(id[i], (void *)&sum[i]);
    }
    printf("%d", sum[0]);
    return 0;
}
