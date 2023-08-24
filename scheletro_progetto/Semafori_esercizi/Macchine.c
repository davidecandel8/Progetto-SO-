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

#define N_Macchine 5

int main (int argc, char * argv[]) {
	pid_t child_pid;
	int status;
    int semaphore = atoi(argv[1]);

    printf("Semaforo %d \n", semaphore);
	
	for (int i = 0; i < N_Macchine; i++)
    {
        switch (fork())
        {
        case -1:
            printf("Errore fork()\n");
            exit(EXIT_FAILURE);
        break;

        case 0:
            printf("Macchina numero %d in attesa ... \n", i);

            reserveSem(semaphore, 0);

			printf("Macchina %d parcheggiata \n", i);
            sleep(2);
            printf("Macchina numero %d lascia il parcheggio\n", i);

            releaseSem(semaphore, 0);
            
		    exit(EXIT_SUCCESS);
		break;

        default:
            break;
        }
    }

    while(wait(&status) != -1){
    }

	exit(EXIT_SUCCESS);
}
