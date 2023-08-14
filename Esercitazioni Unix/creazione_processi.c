#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void stampa(FILE* fp); 
    
int main(){
    FILE* fp = fopen("./nuovofile.txt", "rw"); 
    
    pid_t procPid; 
    switch(procPid=fork()){
        case -1: //errore
            puts("C'è stato un errore nella fork");
            exit(1);
            break;
        case 0: //figlio
            printf("Entra in gioco il processo figlio\n");
            fprintf(fp, "Buongiornissimo Davide, come stai?? (|)\n");
            exit(0);
            break; 
        default: //padre
            printf("Entra in gioco il processo padre\n");
            wait(NULL);
            printf("Fine della wait\n"); 
            stampa(fp); 
            break;        
    }
    fclose(fp); 
    return 0; 
}

void stampa(FILE* fp){
    char buffer[32]; 
        puts(""); 
    while(fgets(buffer, 32, fp)!=NULL){
        printf("%s ", buffer); 
    }
    puts(""); 
}
