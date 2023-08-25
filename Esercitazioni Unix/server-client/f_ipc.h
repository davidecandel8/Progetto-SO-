#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <signal.h> 

#define MY_KEY 1234
#define MSG_LEN 120

struct msgbuf{
    long mtype; 
    char mtext[100];  
};