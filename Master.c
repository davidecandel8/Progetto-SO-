#include <stdio.h> /*fopen*/
#include <sys/types.h> /*Per pid_t (rinomina del tipo int)*/
#include <unistd.h> /*Per la getpid() / execve()*/
#include <sys/wait.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

#define SIZE 30
#define IGNORE -1
#define _GNU_SOURCE


/* errexit.c - errexit __________________ */

#include <stdarg.h>
#include <stdlib.h> /* Def. per la exit() / EXIT_SUCCESS / EXIT_FAILURE */
#include "DataBase.h"

int main(void){
	pid_t procPid, procPidN, procPidP/*, childPid*/;
	int m_id;
	int errExec = 0;
	int count = 0; /*Se siamo in mod. manuale (per l'assegnazione), dice se è stato assegnato un valore a tutti i campi.*/
	int manuale = 0; /*Ci dice che tipo di assegnazione stiamo facendo, ovviamente 0 sta per manuale disattivato*/
	char *f_stringa;
	char *argVec[] = {"navi", NULL}; /*Prova che non serve a nulla. (penso)*/
	char *token; /*usiamo questa varibiale per puntare la meta stringhe prima dell'uguale es. (SO_NAVI=12) token -> SO_NAVI*/
	
	/*in questo modo possiamo usare 12 cifre dopo l'uguale. (Considerando la stringa più lunga)*/
	char parametro[16][30] = {"SO_NAVI=", "SO_PORTI=", "SO_MERCI=", "SO_SIZE=", "SO_MIN_VITA=", "SO_MAX_VITA=", "SO_LATO=", "SO_SPEED=", "SO_CAPACITY=", "SO_BANCHINE=", "SO_FILL=", "SO_LOADSPEED=", "SO_DAYS=", "SO_STORM_DURATION=", "SO_SWELL_DURATION=", "SO_MAELSTROM="};
	
	/*Questa stringhe sono FINAL (Costanti), il valore al loro interno non è modificabile, li usiamo per il confronto tra le stringhe.*/
	char *n_parametro[] = {"SO_NAVI", "SO_PORTI", "SO_MERCI", "SO_SIZE", "SO_MIN_VITA", "SO_MAX_VITA", "SO_LATO", "SO_SPEED", "SO_CAPACITY", "SO_BANCHINE", "SO_FILL", "SO_LOADSPEED", "SO_DAYS", "SO_STORM_DURATION", "SO_SWELL_DURATION", "SO_MAELSTROM"};

	/*Passaggio parametri di configurazione*/
	

	FILE *fd = fopen("./Configurazione.txt", "r");
	char buffer[SIZE]; /*Conterra le stringhe del file di configurazione.*/
	int temp;
	int scelta = IGNORE;
	int i = 0, indice = 0, j;
	char *envVec[17]; /*Array di puntatori che punta alle  variabili del parametro che verra passato al execve come variabile d'ambiente.*/

	if((m_id = msgget(KEY_MASTER_N_P, IPC_CREAT | 0600)) < 0){ /*Creazione coda di messaggi per master porto-navi*/
    	puts("errore durante la creazione di coda di messaggi Master porto-navi!");
    	exit(1);
  	}

	envVec[0] = parametro[0]; /*SO_NAVI*/
	envVec[1] = parametro[1]; /*SO_PORTO*/
	envVec[2] = parametro[2]; /*SO_MERCI*/
	envVec[3] = parametro[3]; /*SO_SIZE*/
	envVec[4] = parametro[4]; /*SO_MIN_VITA*/
	envVec[5] = parametro[5]; /*SO_MAX_VITA*/
	envVec[6] = parametro[6]; /*SO_LATO*/
	envVec[7] = parametro[7]; /*SO_SPEED*/
	envVec[8] = parametro[8]; /*SO_CAPACITY*/
	envVec[9] = parametro[9]; /*SO_BANCHINE*/
	envVec[10] = parametro[10]; /*SO_FILL*/
	envVec[11] = parametro[11]; /*SO_LOADSPPED*/
	envVec[12] = parametro[12]; /*SO_DAYS*/
	envVec[13] = parametro[13]; /*SO_STORM_DURATION*/
	envVec[14] = parametro[14]; /*SO_SWELL_DURATION*/
	envVec[15] = parametro[15]; /*SO_MAELSTROM*/
	envVec[16] = NULL;

    fgets(buffer, SIZE, fd);

    while(indice < SIZE){ /*Questo codice serve per toglire l'accapo se ci fosse o per leggere la prossima stringa se il primo carattere è l'accapo.*/
            if(buffer[indice] == '\n'){
                if(indice == 0){
                    fgets(buffer, SIZE, fd);
                    continue;
                }
                else{
                    buffer[indice] = '\0'; /*Questo è necessario altrimenti strcm() ritorna 10, e le stringhe non avrebbero \0 ma \n*/
                    break;
                }
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

    if(scelta == -1)
	    printf("Inizio della configurazione manuale.\n\n");
    else
        printf("Configurazione scelta: %s\n\n", buffer);
	
	
	switch(scelta){
		case 0:/*dense, small ships:*/
		/*Abbiamo usato parametro al posto di envVec per garantire una protezione*/
			strcat(parametro[0], "1000"); /*SO_NAVI*/
			strcat(parametro[1], "100"); /*SO_PORTO*/
			strcat(parametro[2], "1"); /*SO_MERCI*/
			strcat(parametro[3], "1"); /*SO_SIZE*/
			strcat(parametro[4], "50"); /*SO_MIN_VITA*/
			strcat(parametro[5], "50"); /*SO_MAX_VITA*/
			strcat(parametro[6], "1000"); /*SO_LATO*/
			strcat(parametro[7], "500"); /*SO_SPEED*/
			strcat(parametro[8], "10"); /*SO_CAPACITY*/
			strcat(parametro[9], "2"); /*SO_BANCHINE*/
			strcat(parametro[10], "500000"); /*SO_FILL*/
			strcat(parametro[11], "200"); /*SO_LOADSPPED*/
			strcat(parametro[12], "10"); /*SO_DAYS*/
			strcat(parametro[13], "6"); /*SO_STORM_DURATION*/
			strcat(parametro[14], "24"); /*SO_SWELL_DURATION*/
			strcat(parametro[15], "1"); /*SO_MAELSTROM*/
			break;
		case 1:/*"as bove + trashing":*/
			strcat(parametro[0], "1000"); /*SO_NAVI*/
			strcat(parametro[1], "100"); /*SO_PORTO*/
			strcat(parametro[2], "10"); /*SO_MERCI*/
			strcat(parametro[3], "1"); /*SO_SIZE*/
			strcat(parametro[4], "3"); /*SO_MIN_VITA*/
			strcat(parametro[5], "10"); /*SO_MAX_VITA*/
			strcat(parametro[6], "1000"); /*SO_LATO*/
			strcat(parametro[7], "500"); /*SO_SPEED*/
			strcat(parametro[8], "10"); /*SO_CAPACITY*/
			strcat(parametro[9], "2"); /*SO_BANCHINE*/
			strcat(parametro[10], "500000"); /*SO_FILL*/
			strcat(parametro[11], "200"); /*SO_LOADSPPED*/
			strcat(parametro[12], "10"); /*SO_DAYS*/
			strcat(parametro[13], "6"); /*SO_STORM_DURATION*/
			strcat(parametro[14], "24"); /*SO_SWELL_DURATION*/
			strcat(parametro[15], "1"); /*SO_MAELSTROM*/
			break;
		case 2:/*"born to run":*/
			strcat(parametro[0], "10"); /*SO_NAVI*/
			strcat(parametro[1], "1000"); /*SO_PORTO*/
			strcat(parametro[2], "100"); /*SO_MERCI*/
			strcat(parametro[3], "100"); /*SO_SIZE*/
			strcat(parametro[4], "3"); /*SO_MIN_VITA*/
			strcat(parametro[5], "10"); /*SO_MAX_VITA*/
			strcat(parametro[6], "1000"); /*SO_LATO*/
			strcat(parametro[7], "2000"); /*SO_SPEED*/
			strcat(parametro[8], "1000"); /*SO_CAPACITY*/
			strcat(parametro[9], "10"); /*SO_BANCHINE*/
			strcat(parametro[10], "1000000"); /*SO_FILL*/
			strcat(parametro[11], "500"); /*SO_LOADSPPED*/
			strcat(parametro[12], "10"); /*SO_DAYS*/
			strcat(parametro[13], "6"); /*SO_STORM_DURATION*/
			strcat(parametro[14], "24"); /*SO_SWELL_DURATION*/
			strcat(parametro[15], "60"); /*SO_MAELSTROM*/
			break;
		case 3:/*"cargos, big stuff":*/
			strcat(parametro[0], "100"); /*SO_NAVI*/
			strcat(parametro[1], "5"); /*SO_PORTO*/
			strcat(parametro[2], "10"); /*SO_MERCI*/
			strcat(parametro[3], "100"); /*SO_SIZE*/
			strcat(parametro[4], "3"); /*SO_MIN_VITA*/
			strcat(parametro[5], "10"); /*SO_MAX_VITA*/
			strcat(parametro[6], "1000"); /*SO_LATO*/
			strcat(parametro[7], "500"); /*SO_SPEED*/
			strcat(parametro[8], "1000"); /*SO_CAPACITY*/
			strcat(parametro[9], "10"); /*SO_BANCHINE*/
			strcat(parametro[10], "1000000"); /*SO_FILL*/
			strcat(parametro[11], "200"); /*SO_LOADSPPED*/
			strcat(parametro[12], "10"); /*SO_DAYS*/
			strcat(parametro[13], "6"); /*SO_STORM_DURATION*/
			strcat(parametro[14], "24"); /*SO_SWELL_DURATION*/
			strcat(parametro[15], "24"); /*SO_MAELSTROM*/
			break;
		case 4:/*"unlucky cargos":*/
			strcat(parametro[0], "100"); /*SO_NAVI*/
			strcat(parametro[1], "5"); /*SO_PORTO*/
			strcat(parametro[2], "10"); /*SO_MERCI*/
			strcat(parametro[3], "100"); /*SO_SIZE*/
			strcat(parametro[4], "3"); /*SO_MIN_VITA*/
			strcat(parametro[5], "10"); /*SO_MAX_VITA*/
			strcat(parametro[6], "1000"); /*SO_LATO*/
			strcat(parametro[7], "500"); /*SO_SPEED*/
			strcat(parametro[8], "1000"); /*SO_CAPACITY*/
			strcat(parametro[9], "10"); /*SO_BANCHINE*/
			strcat(parametro[10], "1000000"); /*SO_FILL*/
			strcat(parametro[11], "200"); /*SO_LOADSPPED*/
			strcat(parametro[12], "10"); /*SO_DAYS*/
			strcat(parametro[13], "12"); /*SO_STORM_DURATION*/
			strcat(parametro[14], "10"); /*SO_SWELL_DURATION*/
			strcat(parametro[15], "1"); /*SO_MAELSTROM*/
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

				if(buffer[0] != '\0'){
					token=strtok(buffer, "=");
					while(i < 16){
						if(strcmp(token, *(n_parametro+i))==0){ /*Questo codice serve per collocare il valore nella giusta  posizione se esiste.*/
							scelta = i;
							break; 
						}
						i++;
					}
				}
				/*printf("All'interno del buffer 2 c'è: %s, scelta: %d\n", buffer, scelta); ////////// CODICE DEBUGING*/

				switch(scelta){
					case 0:/*"SO_NAVI":*/
						token = strtok(NULL, "=");
						if(atoi(token) < 1) /*Forza il valore di  SO_NAVI a un minimo di 1*/
							sprintf(token, "1");
						strcat(parametro[0], token);
						count++;
						break;
					case 1:/*"SO_PORTI":*/
						token = strtok(NULL, "=");
						if(atoi(token) < 4) /*Forza il valore di  SO_PORTI a un minimo di 4*/
							sprintf(token, "4");
						strcat(parametro[1], token);
						count++;
						break;
					case 2:/*"SO_MERCI":*/
						strcat(parametro[2], strtok(NULL, "="));
						count++;
						break;
					case 3:/*SO_SIZE":*/
						strcat(parametro[3], strtok(NULL, "="));
						count++;
						break;
					case 4:/*"SO_MIN_VITA":*/
						strcat(parametro[4], strtok(NULL, "="));
						count++;
						break;
					case 5:/*"SO_MAX_VITA":*/
						strcat(parametro[5], strtok(NULL, "="));
						count++;
						break;
					case 6:/*"SO_LATO":*/
						strcat(parametro[6], strtok(NULL, "="));
						count++;
						break;
					case 7:/*"SO_SPEED":*/
						strcat(parametro[7], strtok(NULL, "="));
						count++;
						break;
					case 8:/*"SO_CAPACITY":*/
						strcat(parametro[8], strtok(NULL, "="));
						count++;
						break;
					case 9:/*"SO_BANCHINE":*/
						strcat(parametro[9], strtok(NULL, "="));
						count++;
						break;
					case 10:/*"SO_ FILL":*/
						strcat(parametro[10], strtok(NULL, "="));
						count++;
						break;
					case 11:/*"SO_LOADSPEED":*/
						strcat(parametro[11], strtok(NULL, "="));
						count++;
						break;
					case 12:/*"SO_DAYS":*/
						strcat(parametro[12], strtok(NULL, "="));
						count++;
						break;
					case 13:/*"SO_STORM_DURATION":*/
						strcat(parametro[13], strtok(NULL, "="));
						count++;
						break;
					case 14:/*"SO_SWELL_DURATION":*/
						strcat(parametro[14], strtok(NULL, "="));
						count++;
						break;
					case 15:/*"SO_MAELSTROM":*/
						strcat(parametro[15], strtok(NULL, "="));
						count++;
						break;
                    case IGNORE:
                        break;

				}
				scelta = IGNORE; /*RI-Inizializiamo la variabile per far leggere la prossima stringa*/
				
			} while(fgets(buffer, SIZE, fd) != NULL);
			
            if(manuale && count != 16){ /*Per verificare di aver configurato tutti i parametri*/
				/*errExit("manuale");*/
				puts("Errore: manuale\n");
				exit(0);
			}
            puts("Parametri di Configurazione manuale inseriti con successo.");
	}
	fclose(fd);
	
	/*////////// CODICE DEBUGING
	int inno = 0;
	while(inno < 16){ 
		printf("%s\n\n", parametro[inno]);
                inno++;
    }
    //////////////*/
	
	
	
	
	/*Inizio creazione processi nave e porto*/
    i=0;
    while(i<2){/*Crea i due processi Navi e Porto.*/
        procPid = fork();
        
        if (procPid == 0)
            break;
        else if (procPid == -1){
            puts("Errore fork");
            return 1;
        }
        
        if(i == 0)
            procPidN = procPid;
        else if(i == 1)
            procPidP = procPid;
        
        i++;
    }


	if(procPid == 0){ /*Ambiente del figlio */
		
        if(i == 0){
			sleep(1);
			errExec = execve("./Nave", argVec, envVec);
			printf("sono il processo navi, PID: %d, ci è stato un errore durante execve\n", getpid());
		}else if(i == 1){
			sleep(1);
			errExec = execve("./Porto", argVec, envVec);
			printf("sono il processo porto, PID: %d, ci è stato un errore durante execve\n", getpid());
		}

		printf("Ci è stato un errore imprevisto durante l'execvel (Master!)\n\n\n\n");
		if(errExec == -1){ /*Errori in caso non partisse l'essecutivo*/
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
    }
	for(j=0; j<2; j++){ /*Debug*/
        sleep(1);
        printf("**Master, pid: %d\n\n", getpid());
    }


	while (1){
        if((wait(NULL)) == -1){
            if (errno == ECHILD){
				printf("Fine figli del Master, pid Master: %d\n", getpid()); /*Debug*/
                break;
            }else{
                puts("Ci è stato un errore nella wait");
                return 1;
  		        /*printf("Il processo appena terminato è: %d\n", childPid);*/
            }
        }
    }

    if(msgctl(m_id, IPC_RMID, NULL)<0){ /*Dealoca la coda di messaggi  Master_P_N*/
    	puts("Errore nella chiusara coda di messaggi Master_P_N");
        return 1;
	}

	printf("Questo è il process id di navi: %d, questo invece di porto: %d e questo è il mio %d\n", procPidN, procPidP, getpid()); /*Debug*/
    puts("Processi terminati con successo"); /*Debug*/
	return 0;
}
