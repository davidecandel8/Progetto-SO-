#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#define N_B 3

int main (void) {
	pid_t child_pid;
	int status;

	/*
	printf("%s: My PID is %d and my parent's PID is %d\n",
	       __FILE__, getpid(), getppid());*/
	
	
	for (int i = 0; i < N_B; i++)
    {
        switch (fork())
        {
        case -1:
            printf("Errore fork()\n");
            exit(EXIT_FAILURE);
        break;

        case 0:
			printf("%s: My PID is %d and my parent's PID is %d\n",
	       __FILE__, getpid(), getppid());

		   exit(EXIT_SUCCESS);
		break;

        default:
            
            break;
        }
    }
	
	while (wait(&status) != -1) {
	}

	exit(EXIT_SUCCESS);
}
