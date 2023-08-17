#include <stdio.h>
#include <stdlib.h>  
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "f_ipc.h"

/* 
    Implementare una semplice architettura client-server che rispetti queste
specifiche.
Server:
1. Alloca una coda di messaggi e poi esegue un ciclo di attesa delle richieste dei
processi clienti.
2. Per ogni richiesta ricevuta, esegue una fork() e passa al figlio la richiesta
mentre lui si mette in attesa di una nuova richiesta.
3. Il figlio esamina la richiesta, produce una risposta e la invia al processo
richiedente (nessun altro processo deve poter ricevere tale messaggio), quindi
termina.
4. Se il messaggio ricevuto dal padre non è una richiesta ma un messaggio di
fine, il padre dealloca la coda e termina.
Client:
1. Genera un numero di processi figli pari al numero che viene passato come
argomento dall'utente all'atto dell'esecuzione (da linea di comando, usare argc
e argv).
2. Esegue un ciclo di wait() in attesa della loro terminazione.
3. Ciascun figlio si aggancia a una coda di messaggi allocata dal server.
4. Se la coda non esiste il processo segnala la cosa all'utente e termina.
5. Se la coda esiste, il processo invia una richiesta (decidete voi quale
informazione inserire nel messaggio) e si mette in attesa della risposta.
6. Ricevuta la risposta termina.
7. Quando tutti i figli sono terminati il processo padre informa il server che può
deallocare la coda.
*/

int main(int argc, char ** argv){
    if(argc!=2){
        printf("Usage: <numero figli>\n"); 
        exit(EXIT_FAILURE); 
    }

    int n = atoi(argv[1]); // il numero di processi
    int msqid; 
    size_t pid; 
    
    if(msqid = msgget(MY_KEY, 0666) == -1){
        printf("msgget\n"); 
        exit(1); 
    }
    printf("Connessione eseguita\n"); 

    for(int i = 1; i < n; i++){
        if(pid = fork() <0){ // caso di errore
            printf("Error in fork()\n"); 
            exit(EXIT_FAILURE); 
        }else if(pid == 0){ //figlio
            struct msgbuf richiesta, risposta;

            richiesta.mtype = 123; //----
            
            sprintf(richiesta.mtext, "Richiesta del figlio n° %d\n", i);
            richiesta.mtype = 1; 

            if(msgsnd(msqid, &richiesta, sizeof(richiesta.mtext), 0) < 0){
                printf("Errore in msgsnd\n"); 
                exit(EXIT_FAILURE); 
            }

            if(msgrcv(msqid, &risposta, sizeof(risposta.mtext), getpid(), 0) < 0){
                printf("Error in msgrcv\n"); 
                exit(EXIT_FAILURE); 
            }

            printf("Client %d: Risposta ricevuta: %s\n", getpid(), risposta.mtext); 
            exit(EXIT_SUCCESS);
        }//-----
    }
    //padre
    for(int i=0; i<n; i++)
        wait(NULL);

    struct msgbuf fine_msg; 
    sprintf(fine_msg.mtext, "Fine"); 
    fine_msg.mtype = 1; 

    if(msgsnd(msqid, &fine_msg, sizeof(fine_msg), 0) < 0){
        printf("msgsnd\n");
        exit(EXIT_FAILURE);  
    }

    return 0; 
}