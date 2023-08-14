#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

#define SIZE 32

char** leggi_lista(FILE* file_in, int* nof_elements); 
void stampa_lista(char** mio_ar, int n_elems); 


int main(int argc, char** argv){
    int n = 19;
    int *number = &n;
    FILE *fd = fopen("./lista_nomi.txt", "r");
    char **array_nomi = leggi_lista(fd, number);
    stampa_lista(array_nomi, n); 
    return 0; 
}


char** leggi_lista(FILE* file_in, int* nof_elements){
    char buffer[SIZE];
    char *read_line;

    if(*nof_elements == 0){ //Conteggio degli elementi del a
        //conta gli elementi nel foglio
        int cont=0; 
        while(fgets(buffer, SIZE, file_in)!=NULL)
            cont++; 
        *nof_elements = cont;
        return leggi_lista(fopen("./lista_nomi.txt", "r"), nof_elements); 
    }else{
        
        char **nomi_lista = (char **)malloc((*nof_elements)*sizeof(char*));
        int i=0;
        while(fgets(buffer, SIZE, file_in)!=NULL){
            read_line = malloc((SIZE)*sizeof(char));
            if(read_line == NULL){
                fprintf(stderr,"main: unable to allocate memory for the read line");
                exit(1);
            }
            strcpy(read_line, buffer);
            nomi_lista[i++] = read_line;
            // free(read_line);
        }
        return nomi_lista; 
    }
}

void stampa_lista(char** mio_ar, int n_elems){
    for(int i=0; i<n_elems; i++)
        printf("Nome numero %d: %s\n", (i+1), mio_ar[i]);
}