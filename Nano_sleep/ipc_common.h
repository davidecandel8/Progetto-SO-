
#include <string.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define MSG_LEN 120      /* 128 - sizeof(long) */

typedef struct _merci{
    int type;
    int size;
    int days;
}merci;

struct message_t {
	long mtype;             /* message type, must be > 0 */
	merci mtext;    /* message data */
};

typedef struct _navi{
    int pid;
    double x;
    double y;
}Navi;

typedef struct _porto{
    int pid;
    /*int sem_id;*/
    double x; 
    double y; 
}Porto;

#define SO_NAVI 5
#define SO_LATO 10000
#define SO_SPEED 500 /*km/day*/
#define SO_PORTI 10
#define MY_KEY 123456
