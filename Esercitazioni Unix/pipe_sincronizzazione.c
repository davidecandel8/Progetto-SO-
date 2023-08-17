#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
//+++++++++++++++++++++++++++
#include <errno.h>
#include <signal.h>

#define MAXLINE 32
#define SEC 5

/*
Scrivere un programma seguendo le seguenti istruzioni:
• il genitore crea un pipe (utilizzando la system call pipe()) prima di creare 3
processi figli;
• ogni figlio eredita i descrittori del pipe, e chiude immediatamente il proprio
descrittore per il read end del pipe. L'azione di ogni figlio è semplicemente
mettersi in sleep per un paio di secondi, chiudere anche il descrittore per il
write end e terminare;
• dopo che tutti i figli hanno chiuso i descrittori per il read end del pipe, il
padre effettua un'operazione read() dal pipe ottenendo end-of-file. A tale
fine, consultare il manuale per la funzione read().

PERCHÉ si tratta di una forma di sincronizzazione: il padre NON effettua una
wait() per attendere la terminazione dei figli, ma resta in attesa in lettura dal read
end del pipe.
Notare come la chiusura del write end del pipe da parte del padre è
essenziale al funzionamento di questa tecnica (in caso contrario il padre si
bloccherebbe nel tentativo di leggere dal pipe).
Modificare il programma precedente in modo che prenda in input un numero
arbitrario di interi letti da tastiera: fare attendere il primo figlio argv[1] secondi, il
secondo argv[2] secondi, ..., e l’n-nesimo figlio argv[n] secondi.
*/


int main(int argc, char ** argv){
    int n = argc; 
    int fd[2]; 
    pid_t pid = 1;
    int i;
    char buffer[MAXLINE]; 

    if(pipe(fd)<0)
        puts("pipe error");
    
    for(i=1; i<n; i++){ //Generazione figli
        if((pid > 0)){
            pid = fork();
            if(pid == -1)
                puts("fork error"); 
        }
        if(pid == 0)
            break;
        printf("%d, %d\n", i, getpid());
    }
        
    if(pid > 0){ //padre
        close(fd[1]);
        if(read(fd[0], buffer, MAXLINE)==-1)
            puts("read error"); 
    }else if(pid == 0){ //figlio
        close(fd[0]);
        int j = atoi(argv[i]); 
        for(int x = 0; x < j; x++){
            sleep(1);
            printf("* %d\n", getpid()); 
        }
        close(fd[1]);
        exit(EXIT_SUCCESS);
    }else{
        puts("Errore con la fork");
    }
    printf("Fine\n");
    return 0; 
}