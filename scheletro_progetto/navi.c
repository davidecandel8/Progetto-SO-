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

#include "SMH_H.h"

int main (int argc, char *argv[]) {
	pid_t child_pid;
	int status;
	int MC_PORTI = atoi(argv[0]);
    Porto *shared_data;
    int sem_id;


	sleep(1);// si sostituisce con un semaforo o un segnale

	if(MC_PORTI < 0){
        printf("Errore memoria condivisa in porto\n");
        exit(EXIT_FAILURE);
    }

	//Area di memoria condivisa agganciata
    shared_data = (Porto *)shmat(MC_PORTI, NULL, 0);
	// esempio in cui tutte le navi vanno al primo porto
	sem_id = shared_data[0].sem_id;
	
	//Area di memoria condivisa sganciata
    if(shmdt(shared_data) <0 ){
        printf("Errore shmdt in %d\n", getpid());
        exit(EXIT_FAILURE);
    }
	
	for (int i = 0; i < N_Navi; i++)
    {
        switch (fork())
        {
        case -1:
            printf("Errore fork()\n");
            exit(EXIT_FAILURE);
        break;

        case 0:
			printf("Nave PID %d in attesa ... \n", getpid());

            reserveSem(sem_id, 0);

			/* coda di messaggi */

			printf("Nave PID %d ormeggiata  \n", getpid());
            sleep(1);
            printf("Nave PID %d partita \n", getpid());

            releaseSem(sem_id, 0);

		   exit(EXIT_SUCCESS);
		break;

        default:
            
            break;
        }
    }
	
	while (wait(&status) != -1) {
	}

	exit(EXIT_SUCCESS);
}
