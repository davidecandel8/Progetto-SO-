#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include "DataBase.h"

int main(int argc, char *argv[], char *envp[]){
    size_t pidNavi;
    mes messaggio, cmNavi;
    Nave *navi; 
    int i, j, k;
    int m_id;

    int id_coda; 

    /*Assegnamento dei parametri di configurazione*/
    int SO_NAVI = atoi(convVal(envp[0]));
    int SO_CAPACITY = atoi(convVal(envp[8]));  
    int SO_SPEED = atoi(convVal(envp[7])); 
    int SO_LOADSPEED = atoi(convVal(envp[11]));
    int SO_LATO = atoi(convVal(envp[6]));
    
    /*Creazione e attaccamento della memoria condivisa*/
    int shmid = shmget(IPC_PRIVATE, SO_NAVI * sizeof(Nave),  S_IRUSR | S_IWUSR/*IPC_CREAT*/ | 0666);
    
    if (shmid == -1) {
        perror("Errore nella shmget");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
    }

    if((m_id = msgget(KEY_MASTER_N_P, 0644)) < 0){ 
        puts("errore! Non esiste la coda del key specificato: Master-Nave");
        exit(1);
    }

    if((id_coda = msgget(IPC_PRIVATE, /*IPDC_CREAT*/ 0600))<0){
        perror("Errore nella msgget");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
    }

    navi = (Nave *)shmat(shmid, NULL, 0);
    if (navi == (void *)-1) {
        perror("Errore nell'attaccamento alla memoria condivisa");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
    }

    /*Creazione delle SO_NAVI*/
    for(i=0; i<SO_NAVI; i++){
        switch(pidNavi = fork()){
            case -1: 
                perror("Errore nella fork");
                fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
                exit(EXIT_FAILURE);
            case 0: /*processi navi*/ 
                srand(getpid()); /*Inizializzazione generatore numeri casuali*/
                navi[i].pid = getpid();
                navi[i].x = (double)(rand() % SO_LATO);
                navi[i].y = (double)(rand() % SO_LATO);
                printf("Nave numero %d, con pid %d, posizione x: %.2f, posizione y: %.2f\n", i, navi[i].pid, navi[i].x, navi[i].y);
                
                for(j=0; j<2; j++){ /*Debug*/
                    sleep(1);
                    printf("**Nave, pid: %d\n\n", getpid());
                }

                cmNavi.mtype = 15;
                cmNavi.mes_s.pid = getpid(); 
                sprintf(cmNavi.mes_s.mtext, "Messaggio dal processo nave figlio %d\n", getpid()); 
                /*Invio del messaggio tramite coda di messaggi da parte del processo nave_figlio*/
                if(msgsnd(id_coda, &cmNavi, sizeof(messaggio)-sizeof(long), IPC_NOWAIT)<0){
                    perror("Errore nella msgnd - NAVE");
                    fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
                    exit(EXIT_FAILURE);
                } 

                printf("Fine figlio pid: %d - Nave\n", getpid()); /*Debug*/
                exit(EXIT_SUCCESS); 
        }
    }

    /* Padre deve attendere di leggere tutti i messaggi inviati dai figli */

    for(k=0; k<SO_NAVI; k++){       
        if (msgrcv(id_coda, &cmNavi, sizeof(messaggio)-sizeof(long), 0, 0) == -1) {
            perror("Errore nella msgrcv - Processo Nave");
            fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
            exit(EXIT_FAILURE);
        }
        printf("Messaggio ricevuto dal processo padre-Nave: \"%s\"\n", cmNavi.mes_s.mtext); 
    } 

    while(1){ /*Geestione coda di messaggi*/

        if(msgrcv(m_id, &messaggio, (sizeof(messaggio)-sizeof(long))/*size*/, 0, 0)<0){ /*IPC_NOWAIT*/
            puts("msgrcv error  Master-Nave");
            exit(1);
        }
        
        if(messaggio.mtype == 11){
            puts("Ricevuto messaggio di fine");
            break;
        }
        
        if(messaggio.mes_s.pid < 0){
            puts("il pid passato non è valido");
            exit(1);
        }

        messaggio.mtype = messaggio.mes_s.pid; /*In modo tale che il client elabori solo i messagii indirizzati a lui*/
      
        if((messaggio.mes_s.val % 2) == 0){ 
            strcpy(messaggio.mes_s.mtext, "Pari"); /*sprintf(messaggio.mtext, "Pari");*/
        }else
            strcpy(messaggio.mes_s.mtext, "Dispari"); /*sprintf(messaggio.mtext, "Parco giochi");*/
    
        if(msgsnd(m_id, &messaggio, (sizeof(messaggio)-sizeof(long))/*size*/, IPC_NOWAIT)<0){ /*mtype all'interno di messaggi è mantenuto, il mes è inviato univocalmente allo stesso processo.*/
            puts("coda piena Master-Nave");
            exit(1);
        }

        /*puts("Finitoooooo, sono il processo figlio"); */
        
  }
    
    while(1){
        if(wait(NULL) == -1){
            if(errno == ECHILD){
                puts("Processi figli Navi terminato"); /*Debug*/
                break;
            } else{
                perror("Errore nella wait");
                fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
                exit(EXIT_FAILURE);
            }
        }
    }

    if(msgctl(id_coda, IPC_RMID, NULL)<0){ /*Dealoca la coda di messaggi  Master_N-figli*/
    	perror("Errore nella msgctl, coda di messaggi - Processo Nave");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
	}

    /*Detach e rimozione della memoria condivisa*/
    if(shmdt(navi) == -1){
        perror("Errore nel detach della memoria condivisa - Processo Nave ");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
    }

    if(shmctl(shmid, IPC_RMID, NULL) == -1){
        perror("Errore nella rimozione della memoria condivisa - Processo Nave");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
    }

    puts("§§§§§§§§§Processo Padre Nave terminato§§§§§§§§§§"); /*Debug*/

    exit(EXIT_SUCCESS); 
    /*NOTA: bisogna sincronizzare la Memoria Condivisa con dei semafori*/
}