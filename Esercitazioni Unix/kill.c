#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/errno.h>

int main(){ 
    pid_t processPid, childPid; 
    
    switch (processPid=fork()){
    case -1:
        printf("Errore nella fork\n"); 
        exit(EXIT_FAILURE); 
        break;
    case 0: //figlio
        int value=10;
        printf("Sono entrato nel processo figlio\n"); 
        for(int x=0;x<value;x++){
		    sleep(1);
      		printf("** FIGLIO %d\n", getpid());
	    }
        exit(EXIT_SUCCESS); 
        break;    
    default:
        printf("Sono entrato nel processo padre\n"); 
        int value1=3;
        for(int y=0;y<value1;y++){
		    sleep(1);
      		printf("** PADRE, figlioId: %d\n", processPid);
	    }
        int errore = kill(processPid, SIGKILL); 
        printf("kill: %d\n", errore); 
        if(errore == -1){
            puts("Imbecilleeeee\n");
        }
        if(errno == ESRCH)
            puts("Il processo che vuoi uccidere è fottuto!\n");
        
        childPid = wait(NULL);
        printf("%d\n", childPid); 
        if(errno == ECHILD)
            puts("Il processo padre non  ha più figli\n");
        break;
    }
    return 0;
}
