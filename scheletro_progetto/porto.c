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

#include "SMH_H.h"

int main (int argc, char *argv[]) {
	pid_t child_pid;
	int status;
    int MC_PORTI = atoi(argv[0]);
    Porto *shared_data;
    int sem_id;

    /* MERCI DOMANDA E OFFERTA */

    if(MC_PORTI < 0){
        printf("Errore memoria condivisa in porto\n");
        exit(EXIT_FAILURE);
    }
	
	for (int i = 0; i < N_PORTI; i++)
    {
        switch (fork())
        {
        case -1:
            printf("Errore fork()\n");
            exit(EXIT_FAILURE);
        break;

        case 0:

            //creazione del semaforo di getpid()_porto
            sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
            //Inizializzazione del semaforo
            if(initSemN(sem_id, 0, N_PANCHINE)<0){
                printf("Errore semafori in %d\n", getpid());
                exit(EXIT_FAILURE);
            }

            //Area di memoria condivisa agganciata
            shared_data = (Porto *)shmat(MC_PORTI, NULL, 0);
            //Aggiornamento del valore
            shared_data[i].pid = getpid();
            shared_data[i].sem_id = sem_id;
            //Area di memoria condivisa sganciata
            if(shmdt(shared_data) <0 ){
                printf("Errore shmdt in %d\n", getpid());
                exit(EXIT_FAILURE);
            }

            /*PAUSE*/

		    exit(EXIT_SUCCESS);
		break;

        default:
            
            break;
        }

    }

	while (wait(&status) != -1) {
	}

    //stampa test
/*    printf("\nStampa della memoria in porto.c\n");
    shared_data = shmat(MC_PORTI, NULL, 0);
    for(int i = 0; i < N_PORTI; i++){
        printf("PID: %d --- SEM: %d\n", shared_data[i].pid, shared_data[i].sem_id);
    }*/

	exit(EXIT_SUCCESS);
}
