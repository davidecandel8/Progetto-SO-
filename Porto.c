#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*#include <sys/sysinfo.h>*/
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
#include <sys/msg.h>


#include "DataBase.h"

int main (int argc, char *argv[], char *envp[]) {
    int i; 
    int j; 
    int k; 
	pid_t child_pid;
    mes messaggio, cmPorti;
	int status;
    int sem_id, m_id, shm_id;
    int id_coda; 
    int n; /*Provisorio prima della sincronizzazione tra processi*/

    /*Assegnamento dei parametri di configurazione*/
    int SO_PORTI = atoi(convVal(envp[1]));
    int SO_MERCI = atoi(convVal(envp[2]));
    int SO_SIZE = atoi(convVal(envp[3]));
    int SO_MIN_VITA = atoi(convVal(envp[4]));
    int SO_MAX_VITA = atoi(convVal(envp[5]));
    int SO_LATO = atoi(convVal(envp[6]));
    int SO_BANCHINE  = atoi(convVal(envp[9]));
    int SO_FILL = atoi(convVal(envp[10])); 
    int SO_LOADSPEED = atoi(convVal(envp[11]));
    int SO_DAYS = atoi(convVal(envp[12]));

    Porto *porti; /*Per la memoria condivasa Master porti e porti*/

    if((m_id = msgget(KEY_MASTER_N_P, 0644)) < 0){ /*Recupero id coda di messaggi*/
        puts("errore! Non esiste la coda del key specificato: Master-Nave");
        exit(1);
    }

    if((id_coda = msgget(IPC_PRIVATE, /*IPDC_CREAT*/ 0600))<0){
        perror("Errore nella msgget");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
    }

    if((shm_id = shmget(IPC_PRIVATE, sizeof(Porto)*SO_PORTI, 0600)) < 0){ /*Creazione memoria condivisa*/
        puts("errore! Non esiste la coda del key specificato: Master-Nave");
        exit(1);
    }

	for (i = 0; i < SO_PORTI; i++){
        switch (fork())
        {
        case -1:
            perror("Errore nella fork");
            fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
            exit(EXIT_FAILURE);
        break;

        case 0:
            /*creazione dei 4 porti agli angoli della mappa*/
            porti = (Porto *)shmat(shm_id,NULL,0); /*Agganciamento della memoria condivisa*/
            if(i==0){
                porti[0].x = 0; 
                porti[0].y = 0; 
            }else if(i==1){
                porti[1].x = SO_LATO; 
                porti[1].y = 0; 
            }else if(i==2){
                porti[2].x = 0; 
                porti[2].y = SO_LATO; 
            }else if(i==3){
                porti[3].x = SO_LATO; 
                porti[3].y = SO_LATO; 
            } else{
                srand(getpid()); /*Inizializzazione generatore numeri casuali*/
                porti[i].x = (double)(rand() % SO_LATO);
                porti[i].y = (double)(rand() % SO_LATO);
            }
            porti[i].pid = getpid(); /*Assegna il valore del pid del porto*/
            porti[i].sem_id = semget(IPC_PRIVATE, 1, 0600); /*creazione del semaforo che rappresenta la banchina del porto*/
            
            if(initSemN(porti[i].sem_id, 0, 2/*(rand() % SO_BANCHINE)*/)<0){ /*Inizializzazione del semaforo*/
                perror("Errore nell'inizializzazione del semaforo");
                fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
                exit(EXIT_FAILURE);
            }
            
            printf("Porto numero %d, con pid %d, posizione x: %.2f, posizione y: %.2f\n", i, porti[i].pid, porti[i].x, porti[i].y); /*Debug*/
            
            /* ------------------------------NEWS-------------------------------------*/
                cmPorti.mtype = 14;
                cmPorti.mes_s.pid = getpid(); 
                sprintf(cmPorti.mes_s.mtext, "Messaggio dal processo porto figlio %d\n", getpid()); 
                /*Invio del messaggio tramite coda di messaggi da parte del processo porto_figlio*/
                if(msgsnd(id_coda, &cmPorti, sizeof(messaggio)-sizeof(long), IPC_NOWAIT)<0){
                    perror("Errore nella msgnd");
                    fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
                    exit(EXIT_FAILURE);
                } 
            /* ----------------------------END_NEWS----------------------------------*/

            if(shmdt(porti) ==-1){
                puts("errore nelllo sganciamento. Master-Porto");
                exit(1);
            }

            for(j=0; j<2; j++){ /*Debug*/
                sleep(1);
                printf("**Porto, pid: %d\n\n", getpid());
            }
            
            printf("Fine figlio pid: %d - Porto\n", getpid()); /*Debug*/

		    exit(EXIT_SUCCESS);
        }
    }

    for(k=0; k<SO_PORTI; k++){       
        if (msgrcv(id_coda, &cmPorti, sizeof(messaggio)-sizeof(long), 0, 0) == -1) {
            perror("Errore nella msgrcv - Processo porto");
            fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
            exit(EXIT_FAILURE);
        }
        printf("Messaggio ricevuto dal processo padre-porto: \"%s\"\n", cmPorti.mes_s.mtext); 
    } 

    n = 0;
    while(1){ /*Gestione coda di messaggi*/
        if(n==11){ /*Messaggio per terminare il processo di elaborazione dei messaggi*/
            messaggio.mtype = n;
            if(msgsnd(m_id, &messaggio, (sizeof(messaggio)-sizeof(long))/*size*/, IPC_NOWAIT)<0){ /*Invio richiesta.*/
            puts("Master-Porto coda piena");
            exit(1);
            }
            break;
        }

        messaggio.mtype = (n+1)*10; /*Il tipo del messaggio è un numero qualsiasi per la nostra applicazione usato per rintracciare  un specifico messaggio.*/
        messaggio.mes_s.val = n;
        messaggio.mes_s.pid = getpid();
        sprintf(messaggio.mes_s.mtext, "Esaminiamo il numero: %d, %d", n,  getpid());
        if(msgsnd(m_id, &messaggio, (sizeof(messaggio)-sizeof(long))/*size*/, IPC_NOWAIT)<0){ /*Invio richiesta.*/
            puts("Master-Porto coda piena");
            exit(1);
        }

        if(msgrcv(m_id, &messaggio, (sizeof(messaggio)-sizeof(long))/*size*/, getpid(), 0)<0){ /*Attesa della risposta.*/
            puts("msgrcv error Master-Porto");
            exit(1);
        }

        /*Codice debug*/
        printf("id: %ld, il numero: %d, è %s\n", messaggio.mtype, n, messaggio.mes_s.mtext);

        n++;
    }

	while (1) {
        if(wait(NULL) == -1){
            
            if(errno == ECHILD){
                puts("Processi figli Porto terminato"); /*Debug*/
                break;
            }else{
                perror("Errore nella wait");
                fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
                exit(EXIT_FAILURE);
            }
        } 
	}

    i=0;
    porti = (Porto *)shmat(shm_id,NULL,0); /*Agganciamento della memoria condivisa*/
    while(i<SO_PORTI){ /*Rimozione dei semafori*/
        if(semctl(porti[i].sem_id, 0, IPC_RMID) < 0){ /*Tenere sotto occhio in base all'implementazione*/
            perror("Errore nella rimozione del semaforo - Porto");
            fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
            exit(EXIT_FAILURE);
        }
        i++;
    }

    if(msgctl(id_coda, IPC_RMID, NULL)<0){ /*Dealoca la coda di messaggi  Master_N-figli*/
    	perror("Errore nella msgctl, coda di messaggi - Porto");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
	}

    if(shmctl(shm_id, IPC_RMID,0)<0){ /*Dealocazione memoria condivisa*/
        perror("Errore nella shmctl");
        fprintf(stderr, "Errore numero %d: %s\n", errno, strerror(errno)); 
        exit(EXIT_FAILURE);
    }

    puts("*******Processo Padre Porto terminato********"); /*Debug*/

	exit(EXIT_SUCCESS);
}
