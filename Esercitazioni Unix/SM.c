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
Scrivere un programma che riceve in input due numeri N ed M; il programma
alloca un’area di SM e poi crea N figli. Ciascun figlio cicla M volte e ad ogni ciclo:
- attende un tempo random tra 0 e 9 secondi;
- accede in mutua esclusione alla SM e scrive dentro il suo PID.
Il padre cicla M volte e ad ogni ciclo:
- attende 5 secondi;
- stampa su terminale il PID che trova scritto nella SM.
Quando un figlio ha finito gli M cicli termina. Quando il padre ha finito gli M cicli
si mette in attesa della terminazione dei figli, rimuove l’area di SM e termina
*/

int main(int argc, char**argv){
    if(argc!=3){
        printf("Usage: <nome_file> <N> <M>\n");
        exit(EXIT_FAILURE); 
    }
    int *shared_data;
    int shid = shmget(IPC_PRIVATE, sizeof(int), S_IRUSR | S_IWUSR);

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);  
    size_t pid; 
    srand(time(NULL));

    if(N < 0 || M < 0){
        printf("Error: N e M devono essere maggiori di 0\n");
        exit(EXIT_FAILURE); 
    }

    for(int i=0; i<N; i++){
        switch (pid = fork()){
        case -1:
            printf("Error in fork\n");
            exit(EXIT_FAILURE);
        case 0: //figlio
            for(int j=0; j<M; j++){
                int random = rand()%10; 
                for(int k=0; k<random; k++){
                    sleep(1); 
                    printf("FIGLIO: j = %d * - sec %d\n", j,  k);
                }
                //accedere in mutua esclusione alla SM e scrive dentro il suo PID
                

                int *shared_data = shmat(shid, NULL, 0);
                *shared_data = getpid();
                if (shmdt(shared_data) == -1) {
                    printf("Errore shmdt PID: %d\n", getpid());
                }
            }
            exit(EXIT_SUCCESS); 
        default:
            for(int j=0; j<M; j++){
                for(int k=0; k<5; k++){
                    sleep(1); 
                    printf("PADRE: j = %d * - sec %d\n", j, k);
                }
                shared_data=shmat(shid, NULL, 0);
                printf("PID memorizzato in SM è %d\n", *shared_data);
                if (shmdt(shared_data) == -1) {
                    printf("Errore shmdt PID: %d\n", getpid());
                }
            }
            wait(NULL); 
            break;
        }
    }
    
    if(shmctl(shid, IPC_RMID, 0) < 0){
        printf("Error in shmctl - cancellazione SM padre\n"); 
        exit(EXIT_FAILURE);    
    }
    return 0; 
}