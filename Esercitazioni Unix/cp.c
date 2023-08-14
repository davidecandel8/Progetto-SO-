#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void stampa(FILE* fp); 

int main(int argc, char *argv[])
{
    pid_t childPid;

    childPid = fork();
    if (childPid == -1){
        puts("C'è stato un errore nella fork");
        exit(1);
    }else if(childPid == 0){
        execl("./copy", argv[0], argv[1], argv[2]);
        printf("ERRORE, la execl non è andata a buon fine\n");
        exit(1); 
    }else{
        wait(NULL);
        FILE *fd = fopen(argv[2], "r");
        stampa(fd);
        fclose(fd);
    }
    return 0;
}

void stampa(FILE* fp){
    char buffer[32]; 
        puts(""); 
    while(fgets(buffer, 32, fp)!=NULL){
        printf("%s", buffer); 
    }
    puts(""); 
}
