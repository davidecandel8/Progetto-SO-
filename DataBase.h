#ifndef DATA_BASE_H_safagasdsafffs
#define DATA_BASE_H_safagasdsafffs

#include <string.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define KEY_MASTER_N_P 5
#define size 50
#define SO_MERCI1 10 
/*
    problema da risolvere su come passare al file database.h SO_MERCI --> soluzione più facile ma meno elegante 
    è quella di esportare direttamente in Porto.c la struct porto
*/

typedef struct mes_struct{
  int val;
  int pid;
  char mtext[size];
}m_struct;

typedef struct _mymsg{
  long mtype;
  m_struct mes_s;
}mes;

/* Memoria condivisa */
typedef struct _porto{
    int pid;
    int sem_id;
    double x; 
    double y; 
    int offerta [SO_MERCI1]; /*quantità di merce offerta da un determinato porto in lotti*/
    int richiesta [SO_MERCI1]; /*quantità di merce richiesta da un determinato porto in lotti*/
}Porto;

typedef struct _merci{
    int indice_merce; /*indice della merce*/ 
    int vita; /*data di scadenza*/
    int ton; /*quantita di merce per lotto (in ton)*/
}merci;

/*Equivale a usare  envp: questo per la portabilità  in sistemi come linux*/
char *convVal(char* val){ 
    strtok(val, "=");
    return strtok(NULL, "=");
}

/* Metodi per i semafori*/
union semun {
	int              val;    /* Value for SETVAL*/
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET*/
	unsigned short  *array;  /* Array for GETALL, SETALL*/
	struct seminfo  *__buf;  /* Buffer for IPC_INFO  (Linux-specific) */
};

typedef union _id{ /*Identificatore per la merce*/
    int intero;
    char tipo[15];
}id;

typedef struct _merce{
    id identificativo;  /*Per identificare che tipo di merce, con id o char[15]*/
	int dimensione; /*in tonellata della merce. Un unità è uguale a tale ton di merce*/
	int vita; /*Data di scadenza della merce*/
} merce;

typedef struct _Nave{ /*Definizione struttura navi*/
    int pid; 
    double x; 
    double y; 
}Nave; 

/* Inizializza semaphore a 1 (i.e., "disponibile")*/
int initSemAvailable(int semId, int semNum) {
    union semun arg;
    arg.val = 1;
    return semctl(semId, semNum, SETVAL, arg);
}
/* Inizializza semaphore a 0 (i.e., "in uso")*/
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

#endif /*DATA_BASE_H_safagasdsafffs*/
