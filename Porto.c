#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*#include <sys/sysinfo.h>*/
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

int main (int argc, char *argv[], char *envp[]) {
    int i; 
    int j; 
	pid_t child_pid;
	int status;
    int sem_id;

    int SO_PORTI = atoi(convVal(envp[1]));
    int SO_MERCI = atoi(convVal(envp[2]));
    int SO_SIZE = atoi(convVal(envp[3]));
    int SO_MIN_VITA = atoi(convVal(envp[4]));
    int SO_MAX_VITA = atoi(convVal(envp[5]));
    int SO_LATO = atoi(convVal(envp[6]));
    int SO_BANCHINE  = atoi(convVal(envp[9]));
    int SO_FILL = atoi(convVal(envp[10])); 
    int SO_LOADSPEED = atoi(convVal(envp[11]));
    int SO_DAYS = atoi(convVal(envp[12]));
    Porto *porti = malloc(SO_PORTI * sizeof(Porto));

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
            porti[i].sem_id = semget(IPC_PRIVATE, 1, 0600); /*creazione del semaforo che rappresenta la banchina del porto*/
            
            if(initSemN(porti[i].sem_id, 0, 2/*(rand() % SO_BANCHINE)*/)<0){ /*Inizializzazione del semaforo*/
                perror("Errore nell'inizializzazione del semaforo");
                fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
                exit(EXIT_FAILURE);
            }
            
            printf("Porto numero %d, con pid %d, posizione x: %.2f, posizione y: %.2f\n", i, porti[i].pid, porti[i].x, porti[i].y); /*Debug*/

            for(j=0; j<2; j++){ /*Debug*/
                sleep(1);
                printf("**Porto, pid: %d\n\n", getpid());
            }

            if(semctl(porti[i].sem_id, 0, IPC_RMID) < 0){ /*Tenere sotto occhio in base all'implementazione*/
                perror("Errore nella rimozione del semaforo");
                fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
                exit(EXIT_FAILURE);
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

    free(porti);

    puts("*******Processo Padre Porto terminato********"); /*Debug*/

	exit(EXIT_SUCCESS);
}
