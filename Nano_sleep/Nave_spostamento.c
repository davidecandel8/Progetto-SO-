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

int potenza(int base, int numeratore){ /* base ^ numeratore*/
    if(numeratore == 0)
        return 1;
    else
        return base * potenza(base, numeratore - 1);
}

double my_root(double x)/*calcol radice quadra di x
                            algoritmo babilonese*/
{
  double guess;
  const double epsilon = 0.00001;

  guess = 1;
  while((guess * guess < x - epsilon) || (guess * guess > x + epsilon)) {
    guess = (x / guess + guess) / 2;
  }

  return guess;
}

double distant(double x_1, double x_2, double y_1, double y_2){
    return(my_root(potenza(x_2 - x_1 ,2) + potenza(y_2 - y_1 ,2)));
}

void viaggio(Navi nave, Porto porto){
    struct timespec tempo, tempo_rimanente;
    double parte_intera, parte_frazionaria;

    /*Calcolo del tempo che ci impiega la nave ad arrivare a destinazione*/
    double days = distant(nave.x, porto.x, nave.y, porto.y)/SO_SPEED;
    printf("Giorni di viaggio %f \n", days);
    /*scomponiamo il tempo di percorrenza in parte intera e parte frazionaria*/
    parte_frazionaria = modf(days, &parte_intera);
    tempo.tv_sec = parte_intera;
    tempo.tv_nsec = parte_frazionaria * potenza(10,9); /* essendo "nano" sleep*/

    /* la nave impiegerà un &tempo per arrivare al porto.
        Se viene fermato da un segnale il tempo rimanente viene 
        salvato in &tempo_rimanente
        */
    nanosleep(&tempo, &tempo_rimanente);
}

int scelta(Navi nave, Porto *porto){
    int min = 0;
    int i = 0;
    for(i = 0; i < SO_PORTI; i++ ){
        if(distant(nave.x, porto[i].x, nave.y, porto[i].y) < distant(nave.x, porto[min].x, nave.y, porto[min].y)){
            min = i;
        }
    }
    return min;
}

int main() {


    int pidNavi;
    int i, end;
    Navi navi[SO_NAVI];
    Porto porti[SO_PORTI];
    int m_id;
    srand(getpid());
    
    /*Creazione Porti (già esistenti)*/
    for (i = 0; i < SO_PORTI; i++){ 
            /*creazione dei 4 porti agli angoli della mappa*/
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
                
                porti[i].x = (double)(rand() % SO_LATO);
                porti[i].y = (double)(rand() % SO_LATO);
            }
            porti[i].pid = getpid(); /*Assegna il valore del pid del porto*/
            /*porti[i].sem_id = semget(IPC_PRIVATE, 1, 0600); /*creazione del semaforo che rappresenta la banchina del porto*/
            printf("Porto numero %d, con pid %d, posizione x: %.2f, posizione y: %.2f\n", i, porti[i].pid, porti[i].x, porti[i].y); /*Debug*/
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

                
                    /*porto più vicino (opsionale)*/
                    end = scelta(navi[i], porti);

                    printf("Nave numero %d partita\n", i);
                    viaggio(navi[i], porti[end]);/* nanosleep*/
                    printf("Nave numero %d arrivata al porto %d \n\n", i, end);

                exit(EXIT_SUCCESS);
        }
    }
    while (1) {
        if(wait(NULL) == -1){
                break;
        } 
	}
   
    return EXIT_SUCCESS;
}