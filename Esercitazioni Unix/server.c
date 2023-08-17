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

int main(){
    int q_id;
    struct msgbuf my_msg;
    long rcv_type = 123; // da modificare

    printf("[%d] Waiting for messages....\n", getpid());

    if(msgget(MY_KEY, IPC_CREAT | 0600) < 0){
        printf("Errore");
        exit(EXIT_FAILURE);
    } // crea la coda di messaggio

    if(msgrcv(q_id, &my_msg, MSG_LEN, rcv_type, 0) < 0){
        printf("Errore\n");
        exit(EXIT_FAILURE);
    } // rimuove un messaggio dalla coda

    printf("(PID=%d): Q_id=%d: msg type=%ld \"%s\" RECEIVED\n",
	       getpid(), q_id, my_msg.mtype, my_msg.mtext);

    exit(EXIT_SUCCESS);
}