#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* 
Scrivere un programma che alloca un semaforo e lo inizializza a 0, quindi forka. Il
processo genitore effettua poi una reserveSem() sul semaforo (da implementare
tramite semop). Il processo figlio esegue la releaseSem() (da realizzare con semop)
sullo stesso semaforo dopo 5 secondi dal suo avvio e termina. Il processo padre,
risvegliato dall'attesa, dealloca il semaforo e termina.
*/

#define SEM_INIT_VAL  0

int releaseSem(int semId, int semNum){
    struct sembuf sops;
    sops.sem_num = semNum;
    sops.sem_op = 1;
    sops.sem_flg = 0;
    return semop(semId, &sops, 1);
}

int reserveSem(int semId, int semNum){
    struct sembuf sops;
    sops.sem_num = semNum;
    sops.sem_op = -1;
    sops.sem_flg = 0;
    return semop(semId, &sops, 1);
}

int main(){
    int s_id;
    size_t pid; 
    struct sembuf sops;
    

    // creazione del semaforo
    s_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if(s_id < 0){
        printf("Errore semget\n");
        exit(EXIT_FAILURE);
    }

    if(semctl(s_id, 0, SETVAL, SEM_INIT_VAL) < 0){
        printf("Errore semctl SETVAL\n");
        exit(EXIT_FAILURE);
    }

    switch (pid = fork()){
    case -1:
        printf("Error in fork\n"); 
        break;
    case 0: //figlio
        printf("Entriamo nel processo figlio\n"); 

        if(releaseSem(s_id, 0) < 0){
            printf("Errore in releaseSem\n");   
            exit(EXIT_FAILURE);  
        } 
        
        printf("ReleaseSem fatta\n"); 
        
        for(int i=0; i<5; i++){
            sleep(1);
            printf("*\n");  

        }
        printf("Processo figlio sta terminando\n");
        exit(EXIT_SUCCESS); 
    default: //padre

        
        printf("Entriamo nel processo padre\n"); 

        if(reserveSem(s_id, 0) < 0){
            printf("Errore in reserveSem\n"); 
            exit(EXIT_FAILURE); 
        }  
        printf("ReserveSem fatta\n"); 

        //wait(NULL);
        //deallocazione del semaforo
        if(semctl(s_id, 0 ,IPC_RMID) < 0){
            printf("Errore semctl IPC_RMID");
            exit(EXIT_FAILURE);
        }
        printf("Deallocazione fatta\n"); 
        printf("Processo padre sta terminando\n");
        break;
    }
    
    printf("Fine\n"); 
    exit(EXIT_SUCCESS);
}