#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/errno.h>

int main(){

    pid_t childPid; 
    switch(childPid=fork()){
        case -1: //errore
            puts("C'è stato un errore nella fork");
            exit(1);
            break;
        case 0: //figlio
            printf("Entra in gioco il processo figlio e ora termina\n");
            exit(0);
            break; 
        default: //padre
            printf("Entra in gioco il processo padre\n");
            for(int x=0;x<10;x++){
		            sleep(1);
      		      printf("**\n");
	        } 
            kill(childPid, SIGKILL);
            wait(NULL);
            kill(childPid, SIGKILL);
            printf("%d\n", errno);
            if(errno == ESRCH){
                printf("Non c'è nessuno da uccidere, ritenta domani\n");
            } 
            break;        
    }
    
    return 0; 
}