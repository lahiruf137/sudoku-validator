#include "sudoku_validate.h"
int main(int argc, char **argv){

	if(argc!=3){
		printf("Usage: sudoku <input_file> <max_delay>\n");
		return -1;
	}
	
	
	
	int buffer1[9][9];
	int buffer2[11]={0,0,0,0,0,0,0,0,0,0,0};
	int counter=0;
	char line[30];
	pid_t pid[11];
	pid_t ppid;
	int i;
	//sem_t sem;
	sem_t *sem = sem_open(SNAME, O_CREAT, 0644, 1);
	
	init_shm(1111);
	write_shm(1111,"0");
	
	ppid=getpid();
	/*	Read input file and fill buffer1	*/
	FILE *fp=fopen(argv[1],"r");
	if(fp==NULL){
		//no input file
		return -1;
	}
	
	FILE *fpl=fopen("log.txt","w");
	if(fpl==NULL){
		//no input file
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
	
	
	/*	Create child processes	*/
	for(i=0;i<11;i++){
		pid[i]=fork();
		if(pid[i]==0){
			break;
		}
	}
	
	
	if(i==0){
		if(validate(buffer1[0],9)==1){
			printf("Validation result from process ID-%d: Row 1 is valid\n",getpid());
			update_counter(1111);
		}
		else{
			char message[256];
			sprintf(message,"validation result from process ID-%d: Row 1 is invalid",getpid());
			log_write("log.txt",message);
		}	
	}
	else if(i==1){
		if(validate(buffer1[1],9)==1){
			printf("Validation result from process ID-%d: Row 2 is valid\n",getpid());
			update_counter(1111);
		}
		else{
			char message[256];
			sprintf(message,"validation result from process ID-%d: Row 2 is invalid",getpid());
			update_counter(1111);
			log_write("log.txt",message);
		}
	}
	else if(i==2){
		if(validate(buffer1[2],9)==1){
			printf("Validation result from process ID-%d: Row 3 is valid\n",getpid());
			update_counter(1111);
		}
		else{
			char message[256];
			sprintf(message,"validation result from process ID-%d: Row 3 is invalid",getpid());
			log_write("log.txt",message);
		}
	}
	else if(i==3){
		if(validate(buffer1[3],9)==1){
			printf("Validation result from process ID-%d: Row 4 is valid\n",getpid());
			update_counter(1111);
		}
		else{
			char message[256];
			sprintf(message,"validation result from process ID-%d: Row 4 is invalid",getpid());
			log_write("log.txt",message);
		}
	}
	else if(i==4){
		if(validate(buffer1[4],9)==1){
			printf("Validation result from process ID-%d: Row 5 is valid\n",getpid());
			update_counter(1111);
		}
		else{
			char message[256];
			sprintf(message,"validation result from process ID-%d: Row 5 is invalid",getpid());
			log_write("log.txt",message);
		}
	}
	else if(i==5){
		if(validate(buffer1[5],9)==1){
			printf("Validation result from process ID-%d: Row 6 is valid\n",getpid());
			update_counter(1111);
		}
		else{
			char message[256];
			sprintf(message,"validation result from process ID-%d: Row 6 is invalid",getpid());
			log_write("log.txt",message);
		}
	}
	else if(i==6){
		if(validate(buffer1[6],9)==1){
			printf("Validation result from process ID-%d: Row 7 is valid\n",getpid());
			update_counter(1111);
		}
		else{
			char message[256];
			sprintf(message,"validation result from process ID-%d: Row 7 is invalid",getpid());
			log_write("log.txt",message);
		}
	}
	else if(i==7){
		if(validate(buffer1[7],9)==1){
			printf("Validation result from process ID-%d: Row 8 is valid\n",getpid());
			update_counter(1111);
		}
		else{
			char message[256];
			sprintf(message,"validation result from process ID-%d: Row 8 is invalid",getpid());
			log_write("log.txt",message);
		}
	}
	else if(i==8){
		if(validate(buffer1[8],9)==1){
			printf("Validation result from process ID-%d: Row 9 is valid\n",getpid());
			update_counter(1111);
		}
		else{
			char message[256];
			sprintf(message,"validation result from process ID-%d: Row 9 is invalid",getpid());
			log_write("log.txt",message);
		}
	}
	else if(i==9){
		buffer2[9]=0;
		int cols[9][9];
		for(int r=0;r<9;r++){
			for(int c=0;c<9;c++){
				cols[r][c]=buffer1[c][r];
			}
		}
		
		for(int c=0;c<9;c++){
			if(validate(cols[c],9)==1){
				buffer2[9]+=1;
				update_counter(1111);
			}
		}
		if(buffer2[9]==9){
			printf("Validation result from process ID-%d: %d of 9 columns are valid\n",getpid(),buffer2[9]);
			
		}
		else{
			char message[256];
			sprintf(message,"validation result from process ID-%d: %d  column(s) are invalid",getpid(),9-buffer2[9]);
			log_write("log.txt",message);
		}
	}
	else if(i==10){
		buffer2[10]=0;
		for(int l=0;l<9;l=l+3){
			for(int k=0;k<9;k=k+3){
				int sub_elements[9]={0,0,0,0,0,0,0,0,0};
				int p=0;
				for(int r=0;r<3;r++){
					for(int c=0;c<3;c++){
						sub_elements[p]=buffer1[r+l][c+k];
						p++;
					}
				}
				if(validate(sub_elements,9)==1){
					buffer2[10]+=1;
					update_counter(1111);
				}
			}
		}
		if(buffer2[10]==9){
			printf("Validation result from process ID-%d: %d of 9 sub-grids are valid\n",getpid(),buffer2[10]);
			
		}
		else{
			char message[256];
			sprintf(message,"validation result from process ID-%d: %d sub-grid(s) are invalid",getpid(),9-buffer2[10]);
			log_write("log.txt",message);
		}
	}
	
	if(getpid()==ppid){
		sleep(atoi(argv[2]));
		char *counter_shm=read_shm(1111);
		if(atoi(counter_shm)==27){
			printf("Puzzle is valid: All %d of 27 checks passed\n",atoi(counter_shm));
		}
		else{
			printf("Puzzle is invalid: %d of 27 checks failed\n",27-atoi(counter_shm));
		}
		free(counter_shm);
	}
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