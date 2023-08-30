#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <math.h>

#include "ipc_common.h"

struct timespec {
    time_t tv_sec;  /*Numero di secondi*/
    long tv_nsec;   /*Numero di nanosecondi (parte frazionaria dei secondi)*/
};


int main() {
 
    struct timespec req, rem;
    int result;

    /* Imposta il tempo di attesa a 3.5 secondi */
    req.tv_sec = 3;
    req.tv_nsec = 500000000; /* 0.5 secondi in nanosecondi */

    /* Attendi il tempo specificato */
     result = nanosleep(&req, &rem);

    if (result == -1) {
        /* La funzione è stata interrotta, utilizza il valore in 'rem' per il tempo rimanente */
        printf("nanosleep interrotta. Tempo rimanente: %ld secondi, %ld nanosecondi\n", rem.tv_sec, rem.tv_nsec);
    } else {
        printf("nanosleep completata\n");
    }

/*    double valore = 3.5;
    double parte_intera, parte_frazionaria;

    // Utilizza la funzione modf per separare la parte intera e la parte frazionaria
    parte_frazionaria = modf(valore, &parte_intera);

    printf("Parte intera: %.1lf\n", parte_intera);       // Stampa 3.0
    printf("Parte frazionaria: %.1lf\n", parte_frazionaria); // Stampa 0.5

    return 0;*/

    return EXIT_SUCCESS;
}