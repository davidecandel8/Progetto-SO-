#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SO_NAVI 10
#define SO_CAPACITY 10 
#define SO_SPEED  500
#define SO_LOADSPEED 200
#define SO_LATO 1000

size_t pidNavi; 
struct Nave *navi; 
int i; 

struct Nave{
    int pid; 
    double x; 
    double y; 
}; 

int main(){
    /*Creazione e attaccamento della memoria condivisa*/
    int shmid = shmget(IPC_PRIVATE, SO_NAVI * sizeof(struct Nave), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Errore nella shmget");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
    }

    navi = (struct Nave *)shmat(shmid, NULL, 0);
    if (navi == (void *)-1) {
        perror("Errore nell'attaccamento alla memoria condivisa");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
    }

    /*Creazione delle SO_NAVI*/
    for(i=0; i<SO_NAVI; i++){
        switch(pidNavi = fork()){
            case -1: 
                perror("Errore nella fork");
                fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
                exit(EXIT_FAILURE);
            case 0: /*processi navi*/ 
                srand(getpid()); /*Inizializzazione generatore numeri casuali*/
                navi[i].pid = getpid();
                navi[i].x = (double)(rand() % SO_LATO);
                navi[i].y = (double)(rand() % SO_LATO);
                printf("Nave numero %d, con pid %d, posizione x: %.2f, posizione y: %.2f\n", i, navi[i].pid, navi[i].x, navi[i].y);
                exit(EXIT_SUCCESS); 
        }
    }

    while(1){
        if(wait(NULL) == -1){
            if(errno == ECHILD){
                break;
            } else{
                perror("Errore nella wait");
                fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
                exit(EXIT_FAILURE);
            }
        }
    }

    /*Detach e rimozione della memoria condivisa*/
    if(shmdt(navi) == -1){
        perror("Errore nel detach della memoria condivisa");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
    }

    if(shmctl(shmid, IPC_RMID, NULL) == -1){
        perror("Errore nella rimozione della memoria condivisa");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS); 
}

/*NOTA: bisogna sincronizzare la Memoria Condivisa con dei semafori*/