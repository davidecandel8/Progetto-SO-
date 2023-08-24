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

#define N_Posti 2
#define COMMAND "gcc Macchine.c -o macchine"


int main () {
	pid_t child_pid;
	int status;
    struct sembuf sops;
    char text[sizeof(int)*8+1];

    if(system(COMMAND)){ // invia il commando al sistema e controlla se è stato eseguito
        printf("COMMAND \"%s\" terminated with exit status 1\n", COMMAND);
        exit(EXIT_FAILURE); 
    }

    //crea il set di N semafori
    int s_id = semget(IPC_PRIVATE, 1 , IPC_CREAT | 0600);

    //inizializzazione
/*   if(initSemAvailable(s_id, 0)<0){
        printf("Errore semafori\n");
        exit(EXIT_FAILURE);
    }*/
    if(initSemN(s_id, 0, N_Posti)<0){
        printf("Errore semafori\n");
        exit(EXIT_FAILURE);
    }
    
    sprintf(text,"%d", s_id);
    char* args[] = {
        "macchine",
		text,
		NULL
	};

    if(fork() == 0){
        execve("./macchine", args, NULL);
    }

    /*
    //incremento il semaforo di N_Posti-1
    sops.sem_num = 0;
    sops.sem_op = N_Posti;
    sops.sem_flg = 0;
    semop(s_id, &sops, 1);
	
    
	for (int i = 0; i < N_Posti; i++)
    {
        switch (fork())
        {
        case -1:
            printf("Errore fork()\n");
            exit(EXIT_FAILURE);
        break;

        case 0:
            reserveSem(s_id, 0);
			printf("Parcheggio numero %d\n", i);
            
		    exit(EXIT_SUCCESS);
		break;

        default:
            
            break;
        }
    }
*/
    while(wait(&status) != -1){
    }


    // dealloca tutti semafori di s_id
    printf("Deallocate %d\n", semctl(s_id, 0, IPC_RMID));
    

	exit(EXIT_SUCCESS);
}
