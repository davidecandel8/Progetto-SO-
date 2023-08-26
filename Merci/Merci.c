#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
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

#define SO_MERCI 10 // SO_MERCI tipi diversi
#define SO_FILL 100 // numero totale di merci (in tonnellate)
#define SO_PORTI 4
#define SO_MIN_VITA 1
#define SO_MAX_VITA 10

typedef struct _merci{
    int type;
    int size;
    int days;
}merci;

typedef struct _porto{
    int id;
    merci domanda;
    merci offerta;
}porto;


void distribuzione_porti(porto *array_porto, int TOT_MERCI){
    /* 
                        resetta i numeri generati dalla rand()
                        in modo tale che ad ogni avvio del programma
                        generi numeri diversi
                        */

    int i = 0;
    while(i < SO_PORTI - 1){
        array_porto[i].id = i;
        array_porto[i].domanda.type = (int)(rand() % SO_MERCI + 1);
        printf("TOT_MERCI: %d\n", TOT_MERCI);
        array_porto[i].domanda.size = (int)(rand() % TOT_MERCI + 1);
                                        /* numero casuale tra SO_MIN_VITA e SO_MIN_VITA*/ 
        printf("array_porto[%d].domanda.size : %d\n",i, array_porto[i].domanda.size );
        array_porto[i].domanda.days = (int)(SO_MIN_VITA + (rand() % (SO_MAX_VITA - SO_MIN_VITA)));

        TOT_MERCI = TOT_MERCI - array_porto[i].domanda.size;
        
        if(TOT_MERCI <= 0 && i < SO_PORTI - 1){
            printf("Riavvio della distribuzione\n");
            distribuzione_porti(array_porto,  (int)SO_FILL);
        }
        i++;
    }

    if(TOT_MERCI != 0){
        array_porto[i].id = i;
        array_porto[i].domanda.type = (int)(rand() % SO_MERCI + 1);
        printf("FINALE TOT_MERCI: %d\n", TOT_MERCI);
        array_porto[i].domanda.size = TOT_MERCI;
                                        /* numero casuale tra SO_MIN_VITA e SO_MIN_VITA*/ 
        printf("array_porto[%d].domanda.size : %d\n",i, array_porto[i].domanda.size );
        array_porto[i].domanda.days = (int)(SO_MIN_VITA + (rand() % (SO_MAX_VITA - SO_MIN_VITA)));
        printf("FINE\n");
    }else{
        printf("Riavvio della distribuzione\n");
        distribuzione_porti(array_porto,  (int)SO_FILL);
    }

    printf("FINE\n");
    return;
}

int main (int argc, char *argv[], char *envp[]) {
    porto array_porto[SO_PORTI];
    srand(time(NULL));
    
    int sum = 0;
    //distribuzione_porti(array_porto, (int)SO_FILL);
    int max_merce = SO_FILL ;
    for(int i = 0; i < SO_PORTI - 1; i++){
        array_porto[i].id = i;
        array_porto[i].domanda.type = (int)(rand() % SO_MERCI + 1);
        array_porto[i].domanda.size = (int)(rand() % (SO_FILL - sum) + 1);
                                        /* numero casuale tra SO_MIN_VITA e SO_MIN_VITA*/ 
        array_porto[i].domanda.days = (int)(SO_MIN_VITA + (rand() % (SO_MAX_VITA - SO_MIN_VITA)));

        sum = sum + array_porto[i].domanda.size;
    }

    array_porto[SO_PORTI - 1].id = SO_PORTI - 1;
    array_porto[SO_PORTI - 1].domanda.type = (int)(rand() % SO_MERCI + 1);
    array_porto[SO_PORTI - 1].domanda.size = SO_FILL - sum;
                                        /* numero casuale tra SO_MIN_VITA e SO_MIN_VITA*/ 
    array_porto[SO_PORTI - 1].domanda.days = (int)(SO_MIN_VITA + (rand() % (SO_MAX_VITA - SO_MIN_VITA)));


    sum = 0;
    for(int i = 0; i < SO_PORTI; i++){
        printf("\nPorto: %d\n", array_porto[i].id);
        printf("Domanda TIPO: %d\n", array_porto[i].domanda.type);
        printf("Domanda QUANTITA': %d\n", array_porto[i].domanda.size);
        printf("Domanda GIORNI: %d\n", array_porto[i].domanda.days);
        sum = sum + array_porto[i].domanda.size;
    }
    printf("\nSUM: %d\n", sum);

}
