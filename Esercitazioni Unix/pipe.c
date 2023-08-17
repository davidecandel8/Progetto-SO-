#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* 
    Completare il programma presentato a lezione come pipe1.c . Come si invoca?
    come passare l'argomento se gli vogliamo far scrivere la stringa seguente?
    ciao a tutti!
    Inserire gli header file necessari alla compilazione e all'esecuzione basandosi sugli
    errori e warning rilevati dal compilatore.
    Modificare il programma in modo che se argc vale 3 (cioè se il programma viene
    invocato con un secondo argomento) il processo figlio esegua un'ulteriore fork()
    e passi al proprio figlio un altro pipe da cui il figlio leggerà. Anche il secondo
    argomento deve essere stampato a video. 
*/

int main(int argc, char *argv[]) {
    int pfd[2]; /* pipe file descriptors [0] --> read - [1] --> write */
    char buf[BUFSIZ];
    size_t numRead;

    if (argc<2 || argc > 3 || strcmp(argv[1], "--help") == 0){
        printf("Usage: %s <stringa_da_inviare>\n", argv[0]); 
        exit(EXIT_FAILURE); /* stampa formato invocazione */
    }
    
    if (pipe(pfd) == -1){/* creo il pipe */
        perror("pipe\n"); 
        exit(EXIT_FAILURE); /* gestione errore */
    }

    switch (fork()) {
        case -1:
            perror("fork\n"); 
            exit(EXIT_FAILURE); /* gestione errore */

        case 0: /* ------ figlio - legge dal pipe -------- */
            if (close(pfd[1]) == -1){ /* chiusura del read end */
                perror("close\n");
                exit(EXIT_FAILURE); /* gestione errore */
            }

            if(argc==3){
                int secondPfd[2]; 
                if(pipe(secondPfd) == -1){
                    perror("second pipe\n"); 
                    exit(EXIT_FAILURE); 
                }

                switch(fork()){
                    case -1: 
                        perror("second fork\n"); 
                        exit(EXIT_FAILURE); 
                    case 0:
                        if (close(secondPfd[1]) == -1){ /* chiusura del read end */
                            perror("close\n");
                             exit(EXIT_FAILURE); /* gestione errore */
                        }
                        for (;;) { /* legge dal pipe, e scrive su stdout */
                            numRead = read(secondPfd[0], buf, BUFSIZ);
                            if (numRead == -1){
                                perror("read\n");
                                exit(EXIT_FAILURE); 
                            }
                            if (numRead == 0)
                                break; /* End-of-file */
                            if(write(STDOUT_FILENO, "ambiente nipote: ", 17) != 17){
                                perror("read\n");
                                exit(EXIT_FAILURE);
                            }

                            if (write(STDOUT_FILENO, buf, numRead) != numRead){
                                perror("write\n");
                                exit(EXIT_FAILURE); 
                            }
                        }     
                        write(STDOUT_FILENO, "\n", 1);
                        if (close(secondPfd[0]) == -1){
                            perror("close\n");
                            exit(EXIT_FAILURE); 
                        }
                        exit(EXIT_SUCCESS);
                        
                    default:
                        if (close(secondPfd[0]) == -1){ /* chiusura del read end */
                            perror("close\n");
                            exit(EXIT_FAILURE); /* gestione errore */
                        }
                        if(write(secondPfd[1],argv[2],strlen(argv[2])) != strlen(argv[2])){
                            perror("write\n"); /* gestione errore */
                            exit(EXIT_FAILURE); 
                        }
                        if (close(secondPfd[1]) == -1) /* il figlio riceverà EOF */{
                            perror("close\n"); /* gestione errore */
                            exit(EXIT_FAILURE); 
                        }
                        wait(NULL); /* attesa della terminazione del figlio */
                }
            }

            for (;;) { /* legge dal pipe, e scrive su stdout */
                numRead = read(pfd[0], buf, BUFSIZ);
                if (numRead == -1){
                    perror("read\n");
                    exit(EXIT_FAILURE); 
                }
                if (numRead == 0)
                    break; /* End-of-file */
                if (write(STDOUT_FILENO, buf, numRead) != numRead){
                    perror("write\n");
                    exit(EXIT_FAILURE); 
                }
            }
            write(STDOUT_FILENO, "\n", 1);
            if (close(pfd[0]) == -1){
                perror("close\n");
                exit(EXIT_FAILURE); 
            }
            exit(EXIT_SUCCESS);

        default: /* ------ padre - scrive sul pipe -------- */
            if (close(pfd[0]) == -1){ /* chiusura del read end */
                perror("close\n");
                exit(EXIT_FAILURE); /* gestione errore */
            }
            if(write(pfd[1],argv[1],strlen(argv[1])) != strlen(argv[1])){
                perror("write\n"); /* gestione errore */
                exit(EXIT_FAILURE); 
            }
            if (close(pfd[1]) == -1) /* il figlio riceverà EOF */{
                perror("close\n"); /* gestione errore */
                exit(EXIT_FAILURE); 
            }
            wait(NULL); /* attesa della terminazione del figlio */
            exit(EXIT_SUCCESS);
    } // end switch
}