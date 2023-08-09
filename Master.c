#include <stdio.h> // fopen
#include <sys/types.h> //Per pid_t (rinomina del tipo int)
#include <unistd.h> //Per la exit() / getpid() / execve() 
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define SIZE 30
#define IGNORE -1
#define _GNU_SOURCE


/* errexit.c - errexit __________________ */

#include <stdarg.h>
#include <stdlib.h> /* Def. di EXIT_SUCCESS, EXIT_FAILURE */
//#include "error_functions.h"


/*int errExit(const char *format, ...){
	va_list	args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}*/

//_______________________________________

int main(){
	pid_t procPid, procPidN, procPidP;
	int errExec = 0;
	int count = 0; //Se siamo in mod. manuale (per l'assegnazione), dice se è stato assegnato un valore a tutti i campi.
	int manuale = 0; //Ci dice che tipo di assegnazione stiamo facendo, ovviamente 0 sta per manuale disattivato
	char *f_stringa;
	char *argVec[] = {"navi", NULL};
	
	/*in questo modo possiamo usare 12 cifre dopo l'uguale. (Considerando la stringa più lunga)*/
	char parametro[16][30] = {"SO_NAVI=", "SO_PORTI=", "SO_MERCI=", "SO_SIZE=", "SO_MIN_VITA=", "SO_MAX_VITA=", "SO_LATO=", "SO_SPEED=", "SO_CAPACITY=", "SO_BANCHINE=", "SO_FILL=", "SO_LOADSPPED=", "SO_DAYS=", "SO_STORM_DURATION=", "SO_SWELL_DURATION=", "SO_MAELSTROM="};
	
	//Questa stringhe sono FINAL (Costanti), il valore al loro interno non è modificabile, li usiamo per il confronto tra le stringhe.
	char *n_parametro[] = {"SO_NAVI", "SO_PORTI", "SO_MERCI", "SO_SIZE", "SO_MIN_VITA", "SO_MAX_VITA", "SO_LATO", "SO_SPEED", "SO_CAPACITY", "SO_BANCHINE", "SO_FILL", "SO_LOADSPPED", "SO_DAYS", "SO_STORM_DURATION", "SO_SWELL_DURATION", "SO_MAELSTROM"};

	/*Passaggio parametri di configurazione*/
	

	FILE *fd = fopen("./Configurazione.txt", "r");
	char buffer[SIZE];
	int temp;
	int scelta = IGNORE;
	int i = 0, indice = 0;

	char *envVec[17];
	envVec[0] = parametro[0]; //SO_NAVI
	envVec[1] = parametro[1]; //SO_PORTO
	envVec[2] = parametro[2]; //SO_MERCI
	envVec[3] = parametro[3]; //SO_SIZE
	envVec[4] = parametro[4]; //SO_MIN_VITA
	envVec[5] = parametro[5]; //SO_MAX_VITA
	envVec[6] = parametro[6]; //SO_LATO
	envVec[7] = parametro[7]; //SO_SPEED
	envVec[8] = parametro[8]; //SO_CAPACITY
	envVec[9] = parametro[9]; //SO_BANCHINE
	envVec[10] = parametro[10]; //SO_FILL
	envVec[11] = parametro[11]; //SO_LOADSPPED
	envVec[12] = parametro[12]; //SO_DAYS
	envVec[13] = parametro[13]; //SO_STORM_DURATION
	envVec[14] = parametro[14]; //SO_SWELL_DURATION
	envVec[15] = parametro[15]; //SO_MAELSTROM
	envVec[16] = NULL;
        
        fgets(buffer, SIZE, fd);
        while(indice < SIZE){ /*Questo codice serve per toglire l'accapo se ci fosse */
                if(buffer[indice] == '\n'){
                        buffer[indice] = '\0';
                        break;
                }
                indice++;
        }

	if(strcmp(buffer, "dense, small ships")==0)		
		scelta = 0;
	else if(strcmp(buffer, "as bove + trashing")==0)
		scelta = 1;
	else if(strcmp(buffer, "born to run")==0)
		scelta = 2;
	else if(strcmp(buffer, "cargos, big stuff")==0)
		scelta = 3;
	else if(strcmp(buffer, "unlucky cargos")==0)
		scelta = 4;

	printf("All'interno del buffer c'è: %s\n", buffer);
	
	
	switch(scelta){
		case 0://dense, small ships:
		/*Abbiamo usato parametro al posto di envVec per garantire una protezione*/
			strcat(parametro[0], "1000"); //SO_NAVI
			strcat(parametro[1], "100"); //SO_PORTO
			strcat(parametro[2], "1"); //SO_MERCI
			strcat(parametro[3], "1"); //SO_SIZE
			strcat(parametro[4], "50"); //SO_MIN_VITA
			strcat(parametro[5], "50"); //SO_MAX_VITA
			strcat(parametro[6], "1000"); //SO_LATO
			strcat(parametro[7], "500"); //SO_SPEED
			strcat(parametro[8], "10"); //SO_CAPACITY
			strcat(parametro[9], "2"); //SO_BANCHINE
			strcat(parametro[10], "500000"); //SO_FILL
			strcat(parametro[11], "200"); //SO_LOADSPPED
			strcat(parametro[12], "10"); //SO_DAYS
			strcat(parametro[13], "6"); //SO_STORM_DURATION
			strcat(parametro[14], "24"); //SO_SWELL_DURATION
			strcat(parametro[15], "1"); //SO_MAELSTROM
			break;
		case 1://"as bove + trashing":
			strcat(parametro[0], "1000"); //SO_NAVI
			strcat(parametro[1], "100"); //SO_PORTO
			strcat(parametro[2], "10"); //SO_MERCI
			strcat(parametro[3], "1"); //SO_SIZE
			strcat(parametro[4], "3"); //SO_MIN_VITA
			strcat(parametro[5], "10"); //SO_MAX_VITA
			strcat(parametro[6], "1000"); //SO_LATO
			strcat(parametro[7], "500"); //SO_SPEED
			strcat(parametro[8], "10"); //SO_CAPACITY
			strcat(parametro[9], "2"); //SO_BANCHINE
			strcat(parametro[10], "500000"); //SO_FILL
			strcat(parametro[11], "200"); //SO_LOADSPPED
			strcat(parametro[12], "10"); //SO_DAYS
			strcat(parametro[13], "6"); //SO_STORM_DURATION
			strcat(parametro[14], "24"); //SO_SWELL_DURATION
			strcat(parametro[15], "1"); //SO_MAELSTROM
			break;
		case 2://"born to run":
			strcat(parametro[0], "10"); //SO_NAVI
			strcat(parametro[1], "1000"); //SO_PORTO
			strcat(parametro[2], "100"); //SO_MERCI
			strcat(parametro[3], "100"); //SO_SIZE
			strcat(parametro[4], "3"); //SO_MIN_VITA
			strcat(parametro[5], "10"); //SO_MAX_VITA
			strcat(parametro[6], "1000"); //SO_LATO
			strcat(parametro[7], "2000"); //SO_SPEED
			strcat(parametro[8], "1000"); //SO_CAPACITY
			strcat(parametro[9], "10"); //SO_BANCHINE
			strcat(parametro[10], "1000000"); //SO_FILL
			strcat(parametro[11], "500"); //SO_LOADSPPED
			strcat(parametro[12], "10"); //SO_DAYS
			strcat(parametro[13], "6"); //SO_STORM_DURATION
			strcat(parametro[14], "24"); //SO_SWELL_DURATION
			strcat(parametro[15], "60"); //SO_MAELSTROM
			break;
		case 3://"cargos, big stuff":
			strcat(parametro[0], "100"); //SO_NAVI
			strcat(parametro[1], "5"); //SO_PORTO
			strcat(parametro[2], "10"); //SO_MERCI
			strcat(parametro[3], "100"); //SO_SIZE
			strcat(parametro[4], "3"); //SO_MIN_VITA
			strcat(parametro[5], "10"); //SO_MAX_VITA
			strcat(parametro[6], "1000"); //SO_LATO
			strcat(parametro[7], "500"); //SO_SPEED
			strcat(parametro[8], "1000"); //SO_CAPACITY
			strcat(parametro[9], "10"); //SO_BANCHINE
			strcat(parametro[10], "1000000"); //SO_FILL
			strcat(parametro[11], "200"); //SO_LOADSPPED
			strcat(parametro[12], "10"); //SO_DAYS
			strcat(parametro[13], "6"); //SO_STORM_DURATION
			strcat(parametro[14], "24"); //SO_SWELL_DURATION
			strcat(parametro[15], "24"); //SO_MAELSTROM
			break;
		case 4://"unlucky cargos":
			strcat(parametro[0], "100"); //SO_NAVI
			strcat(parametro[1], "5"); //SO_PORTO
			strcat(parametro[2], "10"); //SO_MERCI
			strcat(parametro[3], "100"); //SO_SIZE
			strcat(parametro[4], "3"); //SO_MIN_VITA
			strcat(parametro[5], "10"); //SO_MAX_VITA
			strcat(parametro[6], "1000"); //SO_LATO
			strcat(parametro[7], "500"); //SO_SPEED
			strcat(parametro[8], "1000"); //SO_CAPACITY
			strcat(parametro[9], "10"); //SO_BANCHINE
			strcat(parametro[10], "1000000"); //SO_FILL
			strcat(parametro[11], "200"); //SO_LOADSPPED
			strcat(parametro[12], "10"); //SO_DAYS
			strcat(parametro[13], "12"); //SO_STORM_DURATION
			strcat(parametro[14], "10"); //SO_SWELL_DURATION
			strcat(parametro[15], "1"); //SO_MAELSTROM
			break;
		default:
			manuale = 1;
			do{     
                                indice = 0;
                                while(indice < SIZE){ /*Questo codice serve per toglire l'accapo se ci fosse */
                                        if(buffer[indice] == '\n'){
                                                buffer[indice] = '\0';
                                                break;
                                        }
                                        indice++;
                                }
				
				if(buffer[0] == '\0')
					while(i < 16){
						if(strcmp(buffer, *(n_parametro+i))==0){
							scelta = i;
							break; 
						} /*else if(buffer[0] == '\0'){
                                                scelta = IGNORE;
                                                break;
                                        }*/
						i++;
					}
				//printf("All'interno del buffer 2 c'è: %s\n", buffer);

				switch(scelta){
					case 0://"SO_NAVI":
						strcat(parametro[0], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 1://"SO_PORTI":
						strcat(parametro[1], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 2://"SO_MERCI":
						strcat(parametro[2], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 3://"SO_SIZE":
						strcat(parametro[3], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 4://"SO_MIN_VITA":
						strcat(parametro[4], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 5://"SO_MAX_VITA":
						strcat(parametro[5], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 6://"SO_LATO":
						strcat(parametro[6], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 7://"SO_SPEED":
						strcat(parametro[7], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 8://"SO_CAPACITY":
						strcat(parametro[8], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 9://"SO_BANCHINE":
						strcat(parametro[9], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 10://"SO_ FILL":
						strcat(parametro[10], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 11://"SO_LOADSPEED":
						strcat(parametro[11], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 12://"SO_DAYS":
						strcat(parametro[12], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 13://"SO_STORM_DURATION":
						strcat(parametro[13], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 14://"SO_SWELL_DURATION":
						strcat(parametro[14], fgets(buffer, SIZE, fd));
						count++;
						break;
					case 15://"SO_MAELSTROM":
						strcat(parametro[15], fgets(buffer, SIZE, fd));
						count++;
						break;
                                        case IGNORE:
                                                break;
					default:
						//errExit("stringa");
						puts("stringa2");
						exit(0);

				}
				scelta = IGNORE; //RI-Inizializiamo la variabile per far leggere la prossima variabile
				//printf("All'interno del buffer 3 c'è: %s\n", buffer);
				
			} while(fgets(buffer, SIZE, fd) != NULL);
			if(manuale && count != 16){
				//errExit("manuale");
				puts("manuale");
				exit(0);

			}
	}

	fclose(fd);
	
	/*puts("Flag");
	exit(0);*/

	
	
	
	/*Inizio creazione processi nave e porto*/

	for(int i = 0; i < 2; i++){
		switch (procPid = fork()) {
			case -1: /* fork() failed */
				//errExit("Fork");
				puts("Fork");
				exit(0);

			case 0: /* Ambiente del figlio */
				if(i == 0){
					sleep(1);
					errExec = execve("./esecutivo", argVec, envVec);
					printf("sono il processo navi, PID: %d\n", getpid());
					//exit(EXIT_SUCCESS);
				}
				/*else if(i == 1){
					sleep(1);
					errExec = execve("./valore_variabile_amb.c", argVec, envVec);
					printf("sono il processo porto, PID: %d\n", getpid());
					//exit(EXIT_SUCCESS);
				}*/

				printf("Ci è stato un errore imprevisto durante l'execvel (Master!)\n\n\n\n");
				if(errExec == -1){ //Errori in caso non partisse l'essecutivo
					switch (errno){
						case EACCES:
							puts("l'argomento pathname non si riferisce a un file normale");
							break;
						case ENOENT:
							puts("Il file riferito dal pathname non esiste.");
							break;
						case ENOEXEC:
							puts("Il file riferito dal pathname è marcato come un eseguibile ma non è riconosciuto come in un formato effettivamente eseguibile.");
							break;
						case ETXTBSY:
							puts("Il file riferito dal pathname è aperto in scrittura da un altro processo.");
							break;
						case E2BIG:
							puts("Lo spazio complessivo richiesto dalla lista degli argomenti e dalla lista dell'ambiente supera la massima dimensione consentita.");
					}
				}
				exit(EXIT_FAILURE);

				break;

			default: /* Ambiente del padre */
				if(i == 0)
					procPidN = procPid;
				else if(i == 1)
					procPidP = procPid;

		}
	}
	pid_t childPid;
	while ((childPid = wait(NULL)) != -1)
  		printf("Il processo appena terminato è: %d\n", childPid);
	
			//printf("PID(figlio): %d\n", getpid()); execve(argv[1], argVec, envVec);

	//if (errno != ECHILD) // errore inatteso errExit("wait");
		//errExit("wait");

	printf("Questo è il process id di navi: %d, questo invece di porto: %d e questo è il mio %d\n", procPidN, procPidP, getpid());
	//wait();
	exit(EXIT_SUCCESS);
}
