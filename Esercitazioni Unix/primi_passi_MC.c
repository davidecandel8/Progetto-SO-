#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/stat.h>
/* 
Si consideri la struttura seguente:

typedef struct my_data {
 int counter;
 char stringa[DATASZ];
} data;

Scrivere un programma in cui il padre svolgerà le seguenti operazioni:

- creazione di un segmento di SM atto a contenere la struttura data;
- aggancio del segmento;
- scrittura all’interno del membro stringa di un messaggio qualsiasi e
inizializzazione del membro counter a 0;
- sganciamento (detach) del segmento.
- dopo avere atteso la terminazione del figlio, il padre rimuoverà il segmento.

Il figlio svolgerà le seguenti operazioni:

- attacco del segmento di SM allo spazio di indirizzi del processo;
- stampa del contenuto dei due membri della struttura condivisa;
- sganciamento (detach) del segmento.

In particolare, riflettere su quando effettuare la fork(), e su soluzioni alternative
per la creazione e il reperimento dell’identificatore del segmento di SM.
*/
#define DATASZ 100

typedef struct my_data {
 int counter;
 char stringa[DATASZ];
} data;

int main(){
    size_t pid;
    struct shmid_ds buffer;
    struct my_data input;

    int shid = shmget(IPC_PRIVATE, sizeof(struct my_data), S_IRUSR | S_IWUSR);
    if(shid == -1){
        printf("Errore shmget\n");
        exit(EXIT_FAILURE);
    }
    printf("creata e inizializzata l'area di memoria condivisa con ID = %d\n", shid);
    
    input.counter = 0;
    sprintf(input.stringa, "Inserimento data\n");

    data *shared_data = shmat(shid, NULL, 0);
    printf("Area di memoria condivisa agganciata.\n");

    *shared_data = input;
    printf("Aggiornato il valore\n");

    if(shmdt(shared_data) == -1){
        printf("Errore shmdt\n");
        exit(EXIT_FAILURE);
    }
    printf("Area di memoria condivisa sganciata.\n");

    switch(pid = fork()){
        case -1: 
            printf("Error in fork\n"); 
            exit(EXIT_FAILURE); 
        case 0: //figlio
            data *shared_data = shmat(shid, NULL, 0);

            if(shmctl(shid, IPC_STAT, &buffer) < 0){
                printf("Error in shmctl - copia da parte del figlio nel buffer\n"); 
                exit(EXIT_FAILURE);    
            }

            printf("Il valore memorizzato è -> %s\n", shared_data->stringa);

            if(shmdt(shared_data) < 0){
                printf("Error in shmdt - sganciamento figlio\n"); 
                exit(EXIT_FAILURE);    
            }
        
            exit(EXIT_SUCCESS); 
        default: //padre
            wait(NULL);
            if(shmctl(shid, IPC_RMID, 0) < 0){
                printf("Error in shmctl - cancellazione SM padre\n"); 
                exit(EXIT_FAILURE);    
            }
            break; 
    }
    
    return 0; 
}