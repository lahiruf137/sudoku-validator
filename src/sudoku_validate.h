#define _XOPEN_SOURCE 700
#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include  <semaphore.h>
#include <pthread.h>
#include <fcntl.h>

#define SHMSZ     27
#define SNAME "/counter"

int isUnique(int arr[],int size);
int isNum(int arr[],int size);
int validate(int arr[],int size);
void log_write(const char *file, char *data);
char *read_shm(int key);
void write_shm(int key,const char * data);
void init_shm(key_t key);
void update_counter(int key);
void *validate_row_pthread(void *ptr);
void *validate_cols_pthread(void *ptr);
void *validate_subgrids_pthread(void *ptr);
extern pthread_mutex_t mutex1;