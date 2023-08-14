#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const* argv[]){
    FILE* fp1 = fopen(argv[1], "r"); 
    printf("Ora stampo argv[]\n"); 
    printf("%s\n", argv[1]); 
    printf("%s\n", argv[2]); 
    FILE* fp2 = fopen(argv[2], "w"); 
    char buffer[32];    

    while(fgets(buffer, 32, fp1)!=NULL)
        fprintf(fp2, "%s", buffer); 
    
    fclose(fp1); 
    fclose(fp2); 
    exit(EXIT_SUCCESS); 
}