#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/errno.h>
/* 
Scrivere un programma in cui un processo forka generando un figlio. Il genitore
setta un handler per gestire i segnali relativi ai cambiamenti di stato del figlio.
Verificare che tale handler viene eseguito mediante invio di segnali al figlio da
terminale.
Modificare l’handler ripristinando la disposizione di default, e verificarne l’effetto
mediante invio di segnali al figlio da terminale.
Implementare tali passaggi due volte, sia con signal(), sia con sigaction().
*/

static void dd_signal_handeler(int sig){
    puts("Toccati il buco del culo..");
}

int main(){
    pid_t processPid;
    struct sigaction sa;
    
    sa.sa_handler = dd_signal_handeler; 

    switch (processPid=fork())
    {
    case -1:
        printf("Errore nella fork\n"); 
        exit(EXIT_FAILURE); 
        break;
    case 0: //figlio
        sigaction(SIGINT, &sa, NULL); 
        //if(signal(SIGINT, dd_signal_handeler) == SIG_ERR){
        int value=6;
        printf("Sono entrato nel processo figlio\n"); 
        for(int x=0;x<value;x++){
		    sleep(1);
      		printf("** FIGLIO %d\n", getpid());
	    }
        exit(EXIT_SUCCESS); 
        break; 
    default: //padre
        printf("Sono entrato nel processo padre\n");
        sigaction(SIGINT, &sa, NULL);
        //if(signal(SIGINT, dd_signal_handeler) == SIG_ERR){
        wait(NULL);
        if(errno == ECHILD)
            puts("C'è stato un errore\n");
        break;
    }

    for(int k=0; k<13; k++){
        sleep(1); 
        printf("%d\n", k); 
    }
    return 0; 
}


