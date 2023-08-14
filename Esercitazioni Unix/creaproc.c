#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t ProcPid;
    int n = 0, i = 0, count = 0;
    printf("Inserisci il numero di figli da creare: ");
    scanf("%d", &n);
    if (n < 3)
        n = 3;

    while (i < n){
        ProcPid = fork();
        if (ProcPid == 0)
            break;
        else if (ProcPid == -1){
            printf("Errore fork\n");
            exit(1);
        }
        i++;
    }
    switch (ProcPid){
    case -1:
        printf("Errore fork\n");
        exit(1);
    case 0: // figlio
        printf("Entra in gioca il figlio\n");
        printf("ProcPid per la sleep: %d\n", getpid());
        printf("Tempo di attesa per la sleep: %d\n", getpid() % 10);
        int value = getpid() % 10;
        for(int x=0;x<value;x++){
		        sleep(1);
      		  printf("** %d\n", getpid());
	      }
        exit(0);
        break;
    default: // padre
        while(count < n){
          printf("Process id:%d\n", wait(NULL));
          count++;
        }
        break;
    }
    return 0;
}
