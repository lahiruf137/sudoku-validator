#include "sudoku_validate.h"

struct Puzzle{
	int buffer[9][9];
	int inverted[9][9];
	int row;
	int col;
	int grid;
	int buffer2[11];
	int counter;
	pthread_mutex_t mutex;
	int threadID[11];
};
int main(int argc, char **argv){

	if(argc!=3){
		printf("Usage: sudoku <input_file> <max_delay>\n");
		return -1;
	}
	
	
	
	int buffer1[9][9];
	char line[30];

	FILE *fp=fopen(argv[1],"r");
	if(fp==NULL){
		return -1;
	}
	
	FILE *fpl=fopen("log.txt","w");
	if(fpl==NULL){
		return -1;
	}
	fclose(fpl);
	
	for(int row=0;row<9;row++){
		if(fgets(line,sizeof(line),fp)!=NULL){
			char *str=strtok(line," ");
			for(int col=0;col<9;col++){
				buffer1[row][col]=atoi(str);
				str=strtok(NULL," ");
			}
		}
		else{
			//input file does has fewer than 9 rows
			return -1;
		}
	}
	fclose(fp);

	struct Puzzle p1;
	pthread_mutex_init(&p1.mutex, NULL);
	p1.counter=0;
	for(int i=0;i<9;i++){
			for(int j=0;j<9;j++){
				p1.buffer[i][j]=buffer1[i][j];
			}
	}
	pthread_t threadID[11];
	
	//Validate rows
	
	for(int c=0;c<9;c++){
		p1.row=c;
		int tid=pthread_create(&threadID[c], NULL, validate_row_pthread, &p1);
		usleep(atoi(argv[2])*100);

	}
	
	//Validate columns
	pthread_create(&threadID[9], NULL, validate_cols_pthread, &p1);
	
	//Validate subgrids
	pthread_create(&threadID[10], NULL, validate_subgrids_pthread, &p1);
	
	sleep(atoi(argv[2]));
	pthread_mutex_lock(&p1.mutex);
	if(p1.counter==27){
		printf("Puzzle is valid: All %d of 27 checks passed\n",p1.counter);
	}
	else{
		printf("Puzzle is invalid: %d of 27 checks failed\n",27-p1.counter);
	}
	pthread_mutex_unlock(&p1.mutex);
	return 0;
}

int isUnique(int arr[],int size){
	int val=0;
	int occurrences=0;
	for(int i=0;i<size;i++){
		val=arr[i];
		occurrences=0;
		for(int j=0;j<size;j++){
			if (val==arr[j]){
				occurrences++;
			}
		}
		if(occurrences>1){
			return 0;
		}
	}

		return 1;
}
int isNum(int arr[],int size){
	for(int i=0;i<size;i++){
		if(isdigit(arr[i])!=0){
		return 0;
		}
	}
	return 1;
}
int validate(int arr[],int size){
	if(isNum(arr,size)==1){
		if(isUnique(arr,size)==1){
			return 1;
		}
		else{
			//May contains duplicate values
			//printf("May contain duplicate values\n");
			return 0;
		}
	}
	else{
		//May contains none numeric values
		//printf("May contains none numeric values\n");
		return 0;
	}
}

void log_write(const char *file, char *data){
	FILE *f = fopen(file, "a+");
	if (f == NULL){
    	printf("Error opening file!\n");
    	return;
	}
	fprintf(f, "%s\n", data);
	fclose(f);
}

char *read_shm(int key){
	int shmid2;
    char *shm2, *s;
    if ((shmid2 = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
	char *data=malloc(256);
	strcpy(data,shm2);
	shmdt(shm2);
	return data;
}

void write_shm(int key,const char * data){
	int shmid2;
    char *shm2, *s;
    if ((shmid2 = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
	strcpy(shm2,data);
	shmdt(shm2);
}
void init_shm(key_t key){
	char c;
    int shmid;
    char *shm, *s;

    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    shmdt(shm);
}

void update_counter(int key){
	sem_t *sem = sem_open(SNAME, 0); 
	sem_wait(sem);
	char *value=read_shm(key);
	int i=atoi(value);
	free(value);
	i++;
	char str[22];
	sprintf(str,"%d",i);
	write_shm(key,str);
	sem_post(sem);
}

void *validate_row_pthread(void *ptr){
	struct Puzzle *pz=ptr;
	pthread_mutex_lock(&pz->mutex);
	int val=validate(pz->buffer[pz->row],9);
	
	if(val==1){
		pz->buffer2[pz->row]=1;
		pz->counter++;
		printf("Validation result from Thread ID-%d: Row %d is valid\n",(int) pthread_self(),pz->row+1);
	}
	else{
		pz->buffer2[pz->row]=0;
		char message[256];
		sprintf(message,"validation result from Thread ID-%d: Row %d is invalid",(int) pthread_self(),pz->row+1);
		log_write("log.txt",message);
	}
	
	int *i=&val;
	pthread_mutex_unlock(&pz->mutex);
	return ((void*) (i));
}
void *validate_cols_pthread(void *ptr){
	struct Puzzle *pz=ptr;
	pthread_mutex_lock(&pz->mutex);
	int arr[9][9];
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			arr[i][j]=pz->buffer[j][i];
		}
	}


	int counter=0;
	pz->buffer2[9]=0;
			
	for(int i=0;i<9;i++){
		int val=validate(arr[i],9);
		if(val==1){
			pz->buffer2[9]++;
			pz->counter++;
			counter++;
		}
	}
	pthread_mutex_unlock(&pz->mutex);
	if(counter==9){
			printf("Validation result from Thread ID-%d: %d of 9 columns are valid\n",(int) pthread_self(),pz->buffer2[9]);
			return ((void*) 1);
		}
		else {
			char message[256];
			sprintf(message,"validation result from Thread ID-%d: %d  column(s) are invalid",(int) pthread_self(),9-pz->buffer2[9]);
			log_write("log.txt",message);
			return (void *) 0;
		}
		
}
void *validate_subgrids_pthread(void *ptr){
	struct Puzzle *pz=ptr;
	int counter=0;
	pthread_mutex_lock(&pz->mutex);
	pz->buffer2[10]=0;
	pthread_mutex_unlock(&pz->mutex);
		for(int l=0;l<9;l=l+3){
			for(int k=0;k<9;k=k+3){
				int sub_elements[9]={0,0,0,0,0,0,0,0,0};
				int p=0;
				for(int r=0;r<3;r++){
					for(int c=0;c<3;c++){
						sub_elements[p]=pz->buffer[r+l][c+k];
						p++;
					}
				}
				if(validate(sub_elements,9)==1){
					counter++;
					pthread_mutex_lock(&pz->mutex);
					pz->counter++;
					pz->buffer2[10]++;
					pthread_mutex_unlock(&pz->mutex);
				}
			}
		}

			pthread_mutex_lock(&pz->mutex);
			if(pz->buffer2[10]==9){
				printf("Validation result from Thread ID-%d: %d of 9 sub-grids are valid\n",(int) pthread_self(),pz->buffer2[10]);
			}
			else{
				char message[256];
				sprintf(message,"validation result from Thread ID-%d: %d sub-grid(s) are invalid",(int) pthread_self(),9-pz->buffer2[10]);
				log_write("log.txt",message);
			}
			pthread_mutex_unlock(&pz->mutex);
			
			return (void *) 1;
}