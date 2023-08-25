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

#define TWO 2
#define N_GIORNI 1
#define N_PORTI 5
#define N_PANCHINE 2
#define N_Navi 4

/* Memoria condivisa*/
typedef struct _porto{
    int pid;
    int sem_id;
}Porto;

/*Metodi per i semafori*/
union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
				    (Linux-specific) */
};

/*Inizializza semaphore a 1 (i.e., "disponibile")*/
int initSemAvailable(int semId, int semNum) {
    union semun arg;
    arg.val = 1;
    return semctl(semId, semNum, SETVAL, arg);
}
/*Inizializza semaphore a 0 (i.e., "in uso")*/
int initSemInUse(int semId, int semNum) {
    union semun arg;
    arg.val = 0;
    return semctl(semId, semNum, SETVAL, arg);
}
/*Inizializza semaphore a n (elementi)*/
int initSemN(int semId, int semNum, int n) {
    union semun arg;
    arg.val = n;
    return semctl(semId, semNum, SETVAL, arg);
}
/*Riservare semaphore - decrementa di 1*/
int reserveSem(int semId, int semNum) {
    struct sembuf sops;
    sops.sem_num = semNum;
    sops.sem_op = -1;
    sops.sem_flg = 0;
    return semop(semId, &sops, 1);
}
/*Rilascia semaphore - incrementa di 1*/
int releaseSem(int semId, int semNum) {
    struct sembuf sops;
    sops.sem_num = semNum;
    sops.sem_op = 1;
    sops.sem_flg = 0;
    return semop(semId, &sops, 1);
}



