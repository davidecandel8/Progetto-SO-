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

void alarmHandler(int sig) {
  	printf("\nSONO PASSATI %d GIORNI\n", N_GIORNI);
    /*
        il segnale è inviato a tutti i processi per i quali
        il processo "Master" ha i permessi di inviare un segnale,
        eccetto init( cha ha pid 1) ed il chiamante (che in questo caso
        non esiste)
    */
  	kill(-1,SIGTERM); 
}

int main (void) {
	pid_t child_pid;
	int status;
    Porto array_porto[N_PORTI];

    #define COMMAND1 "gcc porto.c -o porto"
	status = system(COMMAND1);
    #define COMMAND2 "gcc navi.c -o navi"
	status = system(COMMAND2);

    char text[sizeof(int)*8+1]; // variabile utilizzata per la trasformazione 
                                //di numeri in stringhe

    // crea una memoria condivisa in cui si salvano i pid e i semafori di ogni porto
    int MC_PORTI = shmget(IPC_PRIVATE, N_PORTI*sizeof(Porto), S_IRUSR | S_IWUSR);
    if (MC_PORTI == -1) {
        printf("Errore in semget Master");
        exit(EXIT_FAILURE);
    }
    
    sprintf(text,"%d",MC_PORTI); // il numero preso da MC_PORTI viene trasformato in una stringa
            // per essere inviato al processo ./porto e ./navi
    char* args[] = {
        text,
		NULL
	};
    
    // Creiamo l'alarmHadler che dopo N_giorni
    // termina tutti i processi creati da Master
    struct sigaction sa;
    bzero(&sa, sizeof(sa)); /* set all bytes to zero */
	/* a pointer to the function handling the signal */
	sa.sa_handler = alarmHandler;
	/* Now, setting the handler */
	sigaction (SIGALRM, &sa, NULL);
    alarm(N_GIORNI);


	printf("%s: My PID is %d and my parent's PID is %d\n\n",
	       __FILE__, getpid(), getppid());

           /* 
                i processi esterni vengono chiamati
                dai processi figli creati dalle fork()
                in modo tale che il processo padre possa
                concludere il processo principale
            */
        for (int i = 0; i < TWO; i++)
        {
            switch (fork())
            {
            case -1:
                printf("Errore fork()\n");
                exit(EXIT_FAILURE);
            break;

            case 0:
                if(i == 0){
                    execve("./porto",args,NULL);
                }else{
                    execve("./navi",args,NULL);
                }
                break;

            default:
            break;
            }
        }

    while (wait(&status) != -1) {
	}


    // dealocazione memoria condivisa
    if (shmctl(MC_PORTI, 0, IPC_RMID) == -1) {
        printf("Errore Master shmctl\n");
        exit(EXIT_FAILURE);
    }

	printf("\nREPORT FINALE\n");
	exit(EXIT_SUCCESS);
}
