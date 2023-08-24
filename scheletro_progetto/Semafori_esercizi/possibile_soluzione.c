#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_BANCHINE 10  // Numero massimo di banchine nel porto

int num_banchine;  // Numero casuale di banchine nel porto
sem_t sem_banchine; // Semaforo per gestire le banchine come risorse condivise

void *nave(void *arg) {
    int id = *((int *)arg);
    
    // Richiesta di una banchina
    sem_wait(&sem_banchine);
    
    printf("Nave %d: Attracca alla banchina.\n", id);
    
    // Simulazione del tempo di attracco
    sleep(2);
    
    printf("Nave %d: Parte dalla banchina.\n", id);
    
    // Rilascio della banchina
    sem_post(&sem_banchine);
    
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    
    // Inizializzazione del semaforo delle banchine
    sem_init(&sem_banchine, 0, num_banchine);
    
    // Generazione del numero casuale di banchine
    num_banchine = 1 + rand() % MAX_BANCHINE;
    printf("Numero di banchine nel porto: %d\n", num_banchine);
    
    pthread_t navi[num_banchine];
    int i;
    
    // Creazione dei thread delle navi
    for (i = 0; i < num_banchine; i++) {
        int *id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&navi[i], NULL, nave, (void *)id);
    }
    
    // Attesa del completamento dei thread delle navi
    for (i = 0; i < num_banchine; i++) {
        pthread_join(navi[i], NULL);
    }
    
    // Deallocazione del semaforo
    sem_destroy(&sem_banchine);
    
    return 0;
}
