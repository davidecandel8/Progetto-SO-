#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#include "DataBase.h"

//#define SO_PORTI atoi(getenv(envp[1]))
#define SO_MERCI atoi(getenv(envp[2]))
#define SO_SIZE atoi(getenv(envp[3]))
#define SO_MIN_VITA atoi(getenv(envp[4]))
#define SO_MAX_VITA atoi(getenv(envp[5]))
#define SO_LATO atoi(getenv(envp[6]))
#define SO_BANCHINE atoi(getenv(envp[9]))
#define SO_FILL atoi(getenv(envp[10])) 
#define SO_LOADSPEED atoi(getenv(envp[11]))
#define SO_DAYS atoi(getenv(envp[12]))



int main (int argc, char *argv[], char *envp[]) {
    /*Porto porti[10];*/
    exit(0);
    int SO_PORTI = atoi(getenv(envp[1]));
    Porto *porti = malloc(SO_PORTI * sizeof(Porto));
    int i; 
    int j; 
	pid_t child_pid;
	int status;
    int sem_id;

    if(SO_PORTI < 4){
        SO_PORTI = 4;
    }
	
	for (i = 0; i < SO_PORTI; i++){
        switch (fork())
        {
        case -1:
            perror("Errore nella fork");
            fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
            exit(EXIT_FAILURE);
        break;

        case 0:
            /*creazione dei 4 porti agli angoli della mappa*/
            if(i==0){
                porti[0].x = 0; 
                porti[0].y = 0; 
            }else if(i==1){
                porti[1].x = SO_LATO; 
                porti[1].y = 0; 
            }else if(i==2){
                porti[2].x = 0; 
                porti[2].y = SO_LATO; 
            }else if(i==3){
                porti[3].x = SO_LATO; 
                porti[3].y = SO_LATO; 
            } else{
                srand(getpid()); /*Inizializzazione generatore numeri casuali*/
                porti[i].x = (double)(rand() % SO_LATO);
                porti[i].y = (double)(rand() % SO_LATO);
            }
            porti[i].pid = getpid(); /*Assegna il valore del pid del porto*/
            porti[i].sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600); /*creazione del semaforo che rappresenta la banchina del porto*/
            
            if(initSemN(porti[i].sem_id, 0, (rand() % SO_BANCHINE))<0){ /*Inizializzazione del semaforo*/
                perror("Errore nell'inizializzazione del semaforo");
                fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
                exit(EXIT_FAILURE);
            }
            
            printf("Nave numero %d, con pid %d, posizione x: %.2f, posizione y: %.2f\n", i, porti[i].pid, porti[i].x, porti[i].y); /*Debug*/

            for(j=0; i<2; i++){ /*Debug*/
                sleep(1);
                printf("**Porto, pid: %d\n\n", getpid());
            }
            
            printf("Fine figlio pid: %d\n", getpid()); /*Debug*/

		    exit(EXIT_SUCCESS);
        }
    }

	while (1) {
        if(wait(NULL) == -1){
            
            if(errno == ECHILD){
                puts("Processi figli Porto terminato"); /*Debug*/
                break;
            }else{
                perror("Errore nella wait");
                fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
                exit(EXIT_FAILURE);
            }
        } 
	}

    for(j=0; i<SO_PORTI; i++){
        if(semctl(porti[j].sem_id, 0, IPC_RMID) < 0){
            perror("Errore nell'inizializzazione del semaforo");
            fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
            exit(EXIT_FAILURE);
        }
    }
    
    free(porti);

    puts("Processo Padre Porto terminato"); /*Debug*/

	exit(EXIT_SUCCESS);
}
