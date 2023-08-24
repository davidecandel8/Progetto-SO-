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

#define N_Processi 5


// Metodi per i semafori
union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
				    (Linux-specific) */
};

// Inizializza semaphore a 1 (i.e., "disponibile")
int initSemAvailable(int semId, int semNum) {
    union semun arg;
    arg.val = 1;
    return semctl(semId, semNum, SETVAL, arg);
}
// Inizializza semaphore a 0 (i.e., "in uso")
int initSemInUse(int semId, int semNum) {
    union semun arg;
    arg.val = 0;
    return semctl(semId, semNum, SETVAL, arg);
}
// Riservare semaphore - decrementa di 1
int reserveSem(int semId, int semNum) {
    struct sembuf sops;
    sops.sem_num = semNum;
    sops.sem_op = -1;
    sops.sem_flg = 0;
    return semop(semId, &sops, 1);
}
// Rilascia semaphore - incrementa di 1
int releaseSem(int semId, int semNum) {
    struct sembuf sops;
    sops.sem_num = semNum;
    sops.sem_op = 1;
    sops.sem_flg = 0;
    return semop(semId, &sops, 1);
}

int main () {
	pid_t child_pid;
	int status;

    //crea il set di "N_Processi" semafori
    int s_id = semget(IPC_PRIVATE, N_Processi, IPC_CREAT | 0600);

    //inizializzazione
    if(initSemInUse(s_id, 0)<0){
        printf("Errore semafori\n");
        exit(EXIT_FAILURE);
    }
	/*
	printf("%s: My PID is %d and my parent's PID is %d\n",
	       __FILE__, getpid(), getppid());*/
	
	//kill(getpid(), SIGTERM);
	
	for (int i = 0; i < N_Processi; i++)
    {
        switch (fork())
        {
        case -1:
            printf("Errore fork()\n");
            exit(EXIT_FAILURE);
        break;

        case 0:
            /* Wait for the "green light" by the parent */
            // Diminuendo di 1 il valore 0 del i semaforo del set s_id
            // (diventando -1), il processo figlio si blocca
            // fino a quando il valore del i semaforo del set s_id
            // non ritorna >= 0
            reserveSem(s_id,i);

			printf("%s: My PID is %d and my parent's PID is %d --- number %d\n",
	       __FILE__, getpid(), getppid(), i);
            
		   exit(EXIT_SUCCESS);
		break;

        default:
            printf("Padre\n");
            break;
        }
    }

    // Incrementa di uno tutti i semafori del set s_id
    // sbloccando così i processi a cui appartengono
    for(int i = N_Processi -1; i >= 0; i--){
        releaseSem(s_id,i);
    }

	while (wait(&status) != -1) {
	}

    // dealloca tutti semafori di s_id
    printf("Deallocate %d\n", semctl(s_id, 0, IPC_RMID));


	exit(EXIT_SUCCESS);
}
