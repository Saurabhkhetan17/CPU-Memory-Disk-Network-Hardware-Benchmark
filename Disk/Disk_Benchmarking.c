#include<stdio.h>
#include<math.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<fcntl.h>
#include<time.h>

struct valPass
{
	long int b_size;
	int no_threads;
} v1;

struct timeval start_new, end_new; // for throughput and latency

double total_time = 0;

void *seq_read(void *v1){
	//file pointer
	FILE *f;
	int i;
	struct valPass *params = v1;
	long int b_size = params -> b_size;
	int no_threads =  params -> no_threads;
	char *buffer = (char*)malloc(b_size);
	long int total_size = 1024*1024*1024*sizeof(char);
	
	//int sz=0;

	f = fopen("disk_file.txt", "r+");
	//fseek(f, 0, SEEK_END);
 	//sz = ftell(f);
 	int no_blocks = total_size/(b_size*no_threads);

 	gettimeofday(&start_new, NULL);
    for(i=0; i<no_blocks; i++){ 	
    	fread(buffer, b_size, 1, f);
    	//fread(buffer,block_size,1,fh);
    }
    gettimeofday(&end_new, NULL);
	double data1=(double)start_new.tv_sec+((double)start_new.tv_usec/1000000);
	double data2=(double)end_new.tv_sec+((double)end_new.tv_usec/1000000);
	total_time = total_time + (data2 - data1);
    fclose(f);
}

void *seq_read_write(void *v1){
	//file pointer
	FILE *f, *fw;
	int i;
	struct valPass *params = v1;
	long int b_size = params -> b_size;
	int no_threads =  params -> no_threads;
	char *buffer = (char*)malloc(b_size);
	long int total_size = 1024*1024*1024*sizeof(char);
	//int sz=0;	
	f = fopen("disk_file.txt", "r");
	fw = fopen("disk_file_new.txt", "w");
	//fseek(f, 0, SEEK_END);
 	//sz = ftell(f);
 	int no_blocks = total_size/(b_size*no_threads);

 	gettimeofday(&start_new, NULL);
	
    for(i=0; i<no_blocks; i++){ 	
    	fread(buffer, b_size, 1, f);
    	fwrite(buffer, b_size, 1, fw);

    	//fread(buffer,block_size,1,fh);
    }
    gettimeofday(&end_new, NULL);
	double data1=(double)start_new.tv_sec+((double)start_new.tv_usec/1000000);
	double data2=(double)end_new.tv_sec+((double)end_new.tv_usec/1000000);
	total_time = total_time + (data2 - data1);
    fclose(f);
    fclose(fw);
}


void *random_read(void *v1){
	//file pointer
	FILE *f;
	int i;
	struct valPass *params = v1;
	long int b_size = params -> b_size;
	int no_threads =  params -> no_threads;
	//int sz=0;
	char *buffer = (char*)malloc(b_size);
	long int total_size = 1024*1024*1024*sizeof(char);

	

	f = fopen("disk_file.txt", "r+");

	//printf("initial Location For File Is: %d\n", f);
	//fseek(f, 0, SEEK_END);
 	//sz = ftell(f);

 	int no_blocks = total_size/(b_size*no_threads);

 	gettimeofday(&start_new, NULL);

    for(i=0; i<no_blocks; i++){ 	
    	unsigned long int rand_temp = rand()%no_blocks;
    	//printf("Rand Temp is%d\n", rand_temp);
    	fseek(f, rand_temp, SEEK_SET);
    	//printf("New Location For File Is: %d\n", f);
    	fread(buffer, b_size, 1, f);
    }
    gettimeofday(&end_new, NULL);
	double data1=(double)start_new.tv_sec+((double)start_new.tv_usec/1000000);
	double data2=(double)end_new.tv_sec+((double)end_new.tv_usec/1000000);
	total_time = total_time + (data2 - data1);

    fclose(f);
}


int main(int argc, char* argv[]){
	int oper;
	long int b_size_temp;
	int no_threads_temp;
	
	int incr,j; // for looping over threads
	oper = atoi(argv[1]);
	b_size_temp = atoi(argv[2]);
	no_threads_temp = atoi(argv[3]);

	v1.b_size = b_size_temp;
	v1.no_threads = no_threads_temp;
	pthread_t new_thread[v1.no_threads]; //thread variable

	if(oper==1){
		printf("The chosen operation is: Sequential Read, Block Size is: %d and Number Of Threads is: %d\n", b_size_temp, no_threads_temp);
		for(incr=0; incr<no_threads_temp; incr++){
			pthread_create(&new_thread[incr], NULL, &seq_read, (void*)&v1);
		}

		for(j=0;j<no_threads_temp;j++)
		{
			pthread_join(new_thread[j], NULL);
		}

		if(b_size_temp==8){
		total_time  = total_time / no_threads_temp;
		float latency = (float)(total_time*b_size_temp*1000)/(1024*1024*1024*sizeof(char));
		printf("Required Latency Is: %f\n", latency);
		}
		else{
		total_time  = total_time / no_threads_temp;
		float throughput = (1024*1024*1024*sizeof(char)/total_time)/(1024*1024);
		printf("Required Througput Value is: %f\n", throughput);
		}
	}
	else if(oper==2){
		printf("The chosen operation is: Sequential Read Write, Block Size is: %d and Number Of Threads is: %d\n", b_size_temp, no_threads_temp);
		
		for(incr=0; incr<no_threads_temp; incr++){
			//printf("incr value is %d\n", incr);
			pthread_create(&new_thread[incr], NULL, &seq_read_write, (void*)&v1);
		}
		for(j=0;j<no_threads_temp;j++)
		{
			pthread_join(new_thread[j], NULL);
		}
		if(b_size_temp==8){
		total_time  = total_time / no_threads_temp;
		float latency = (float)(total_time*b_size_temp*1000)/(1024*1024*1024*sizeof(char));
		printf("Required Latency Is: %f\n", latency);
		}
		else{
		total_time  = total_time / no_threads_temp;
		float throughput = (1024*1024*1024*sizeof(char)/total_time)/(1024*1024);
		printf("Required Througput Value is: %f\n", throughput);
		}	
		
	}

	else if(oper==3){
		printf("The chosen operation is: Random Read , Block Size is: %d and Number Of Threads is: %d\n", b_size_temp, no_threads_temp);
		
		for(incr=0; incr<no_threads_temp; incr++){
			//printf("incr value is %d\n", incr);
			pthread_create(&new_thread[incr], NULL, &random_read, (void*)&v1);
		}
		for(j=0;j<no_threads_temp;j++)
		{
			pthread_join(new_thread[j], NULL);
		}
		if(b_size_temp==8){
		total_time  = total_time / no_threads_temp;
		float latency = (float)(total_time*b_size_temp*1000)/(1024*1024*1024*sizeof(char));
		printf("Required Latency Is: %f\n", latency);
		}
		else{
		total_time  = total_time / no_threads_temp;
		float throughput = (1024*1024*1024*sizeof(char)/total_time)/(1024*1024);
		printf("Required Througput Value is: %f\n", throughput);
		}			
	}
	return 0;
}