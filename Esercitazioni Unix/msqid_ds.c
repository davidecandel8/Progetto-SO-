/*#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <errno.h>*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <time.h>

/*
Scrivere un programma in cui è creata una coda di messaggi.
- Prima di effettuare qualsiasi operazione di invio di messaggi sulla coda,
utilizzando la struttura di tipo msqid_ds associata alla coda, stampare la
dimensione della coda (membro msg_qbytes), il numero di messaggi in coda
(membro msg_qnum) e il tempo dell’ultima msgsnd() (deve essere 0, poiché
non ci sono ancora stati invii sulla coda).
- Effettuare una msgsnd(), e verificare che il momento della msgsnd() è cambiato.
Utilizzare la funzione ctime() di time.h per visualizzare il momento in cui è
occorsa la msgsnd().
- Prima di terminare il programma deve deallocare la coda.
*/

// struct msqid_ds {
// struct ipc_perm msg_perm; /*Ownership e permissions */
// time_t msg_stime; /* Time of last msgsnd() */
// time_t msg_rtime; /* Time of last msgrcv() */
// time_t msg_ctime; /* Time of last change */
// unsigned long __msg_cbytes; /* Number of bytes in
// queue*/
// msgqnum_t msg_qnum; /* Number of messages in queue */
// msglen_t msg_qbytes; /* Maximum bytes in queue */
// pid_t msg_lspid; /* PID of last msgsnd() */
// pid_t msg_lrpid; /* PID of last msgrcv() */
// }

#define MSG_LEN 200

struct msgbuf {
	long mtype;             /* message type, must be > 0 */
	char testo[MSG_LEN];    /* message data */
};

int main(){
    struct msqid_ds mq_info; // definizione della struttura dati della coda "msqid_ds"
    int m_id;

    if ((m_id = msgget(IPC_PRIVATE, /*IPC_CREAT|*/0600)) < 0) { // msgget -> creazione della coda di messaggi
                                                        // IPC_PRIVATE = genera un numero casuale per la key
                                                        // IPC_CREAT = se non esiste una coda con la key specificata, crea
                                                        //             una nuova coda
        printf(" Errore in msgget\n");
        exit(EXIT_FAILURE);
    }

   printf("Creata coda di messaggi con identificatore: %d\n",m_id);

   if(msgctl(m_id, IPC_STAT, &mq_info) < 0){
        printf(" Errore in msgctl\n");
        exit(EXIT_FAILURE);
   }

   printf("Stato di msg_qbytes: %ld\n", mq_info.msg_qbytes); 
   printf("Stato di msg_qnum: %ld\n", mq_info.msg_qnum); 
   printf("Stato di msg_lspid: %d\n", mq_info.msg_lspid); //deve essere 0

   struct msgbuf messaggio;
   messaggio.mtype = 1;
   sprintf(messaggio.testo, "Ciao mondo"); // messaggio.test = array("Ciao mondo")

   if (msgsnd(m_id, &messaggio, MSG_LEN, 0)<0) {
        printf(" Errore in msgsnd\n");
        exit(EXIT_FAILURE);
   }
   printf("Spedito il messaggio\n\n");

   
  if (msgctl(m_id, IPC_STAT, &mq_info)<0) {
    printf("Errore in msgctl ");
    exit(EXIT_FAILURE);
  }
    printf("Stato di msg_qbytes: %ld\n", mq_info.msg_qbytes); 
   printf("Stato di msg_qnum: %ld\n", mq_info.msg_qnum); 
   printf("Stato di msg_lspid: %d\n", mq_info.msg_lspid);

   msgctl(m_id, IPC_RMID, NULL);
   
   printf("Rimossa la coda di messaggi con identificatore: %d\n",m_id);

    return 0; 
}