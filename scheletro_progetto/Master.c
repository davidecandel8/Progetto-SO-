#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#define TWO 2
#define N_GIORNI 3

int main (void) {
	pid_t child_pid;
	int status;

    #define COMMAND1 "gcc A.c -o A"
	status = system(COMMAND1);
    #define COMMAND2 "gcc B.c -o B"
	status = system(COMMAND2);
	
    char* args[] = {
		NULL
	};

	printf("%s: My PID is %d and my parent's PID is %d\n\n",
	       __FILE__, getpid(), getppid());

           /* 
                i processi esterni vengono chiamati
                dai processi figli creati dalle fork()
                in modo tale che il processo padre possa
                concludere il processo principale
            */
        for (int i = 0; i < TWO; i++)
        {
            switch (fork())
            {
            case -1:
                printf("Errore fork()\n");
                exit(EXIT_FAILURE);
            break;

            case 0:
                if(i == 0){
                    execve("./A",args,NULL);
                }else{
                    execve("./B",args,NULL);
                }
                break;

            default:
            break;
            }
        }

    while (wait(&status) != -1) {
	}

	printf("\nREPORT FINALE\n");

	exit(EXIT_SUCCESS);
}
