#include<stdio.h>
#include<math.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/time.h>

struct valPass
{
	long int b_size;
	int no_threads;
};

struct timeval start_new, end_new; // for throughput and latency

double tot_time_new = 0;

void *seq_read_write(void *v1){
	char *src;
	char *dest;
	struct valPass *params = v1;
	long int b_size = params -> b_size;
	int no_threads =  params -> no_threads;
	int j = 0;
	src = (char*)malloc(1024*1024*1024*sizeof(char));
	char *src_temp;
	src_temp = src;
	dest = (char*)malloc(1024*1024*1024*sizeof(char)); //allocate mem and write
	char *dest_temp;
	dest_temp = dest;
	long int no_blocks = 1024*1024*1024*sizeof(char)/(b_size*no_threads);
	gettimeofday(&start_new, NULL);
	for(j=0; j<no_blocks; j++){
		memcpy(dest, src, b_size);
		src = src + b_size;
		dest = dest + b_size;
	}
	gettimeofday(&end_new, NULL);
	double data1=(double)start_new.tv_sec+((double)start_new.tv_usec/1000000);
	double data2=(double)end_new.tv_sec+((double)end_new.tv_usec/1000000);
	tot_time_new = tot_time_new + (data2 - data1);
	src = src_temp;
	dest = dest_temp;
	free(src);
	free(dest);
}

void *seq_write(void *v1){
	char *src;	
	struct valPass *params = v1;
	long int b_size = params -> b_size;
	int no_threads =  params -> no_threads;
	src = (char*)malloc(1024*1024*1024*sizeof(char));
	char *src_temp;
	src_temp = src;
	int j = 0;
	int no_blocks = 1024*1024*1024*sizeof(char)/(b_size*no_threads);
	gettimeofday(&start_new, NULL);
	for(j=0;j<no_blocks;j++){
		memset(src, 'A', b_size);
		src = src + b_size;
	}
	gettimeofday(&end_new, NULL);
	double data1=(double)start_new.tv_sec+((double)start_new.tv_usec/1000000);
	double data2=(double)end_new.tv_sec+((double)end_new.tv_usec/1000000);
	tot_time_new = tot_time_new + (data2 - data1);
	src = src_temp;
	free(src);
}

void *random_write(void *v1){
	char *src1;
	struct valPass *params = v1;
	long int b_size = params -> b_size;
	int no_threads =  params -> no_threads;

	src1 = (char*)malloc(1024*1024*1024*sizeof(char));
	char *src_temp1;
	src_temp1 = src1;
	char *rand_loc; //random loc
	rand_loc = (char*)malloc(1024*1024*1024*sizeof(char));
	int k = 0;	//for looping through number of blocks
	int incr = 0;
	long int no_blocks_new = 1024*1024*1024*sizeof(char)/(b_size*no_threads);
	gettimeofday(&start_new, NULL);
	for(k=0;k<no_blocks_new;k++){
		long int rand_temp = rand()%no_blocks_new;
		rand_loc = src1 + (rand_temp)*b_size;	
		memset(rand_loc, 'A', b_size);
		incr += 1;
	}
	gettimeofday(&end_new, NULL);
	double data1=(double)start_new.tv_sec+((double)start_new.tv_usec/1000000);
	double data2=(double)end_new.tv_sec+((double)end_new.tv_usec/1000000);
	tot_time_new = tot_time_new + (data2 - data1);
	free(src_temp1);
}

int main(int argc, char* argv[]){
	int oper;
	long int b_size_temp;
	int no_threads_temp;
	struct valPass v1;
	int incr,j; // for looping over threads
	oper = atoi(argv[1]);
	b_size_temp = atoi(argv[2]);
	no_threads_temp = atoi(argv[3]);
	v1.b_size = b_size_temp;
	v1.no_threads = no_threads_temp;
	pthread_t new_thread[v1.no_threads]; //thread variable

	if(oper==1){
		printf("The chosen operation is: Sequential Read Write, Block Size is: %d and Number Of Threads is: %d\n", b_size_temp, no_threads_temp);
		for(incr=0; incr<no_threads_temp; incr++){
			pthread_create(&new_thread[incr], NULL, &seq_write, (void*)&v1);
		}

		for(j=0;j<no_threads_temp;j++){
			pthread_join(new_thread[j], NULL);
		}
		if(b_size_temp==8){
			tot_time_new  = tot_time_new / no_threads_temp;
			float latency = (float)(tot_time_new*b_size_temp*1000)/(1024*1024*1024*sizeof(char));
			printf("Latency Is: %f\n", latency);
		}
		else{
			tot_time_new  = tot_time_new / no_threads_temp;
			float th_new = (1024*1024*1024*sizeof(char)/tot_time_new)/(1024*1024);
			printf("Throughput Is: %f\n", th_new);
		}
	}
	else if(oper==2){
		printf("The chosen operation is: Sequential Write, Block Size is: %d and Number Of Threads is: %d\n", b_size_temp, no_threads_temp);
		for(incr=0; incr<no_threads_temp; incr++){
			pthread_create(&new_thread[incr], NULL, &seq_read_write, (void*)&v1);
		}
		for(j=0;j<no_threads_temp;j++)
		{
			pthread_join(new_thread[j], NULL);
		}
		if(b_size_temp==8){
			tot_time_new  = tot_time_new / no_threads_temp;
			float latency = (float)(tot_time_new*b_size_temp*1000)/(1024*1024*1024*sizeof(char));
			printf("Latency Is: %f\n", latency);
		}
		else{
			tot_time_new  = tot_time_new / no_threads_temp;
			float th_new = (1024*1024*1024*sizeof(char)/tot_time_new)/(1024*1024);		
			printf("Throughput Is: %f\n", th_new);
		}	
		
	}

	else if(oper==3){
		printf("The chosen operation is: Random Write , Block Size is: %d and Number Of Threads is: %d\n", b_size_temp, no_threads_temp);
		for(incr=0; incr<no_threads_temp; incr++){
			pthread_create(&new_thread[incr], NULL, &random_write, (void*)&v1);
		}
		for(j=0;j<no_threads_temp;j++){
			pthread_join(new_thread[j], NULL);
		}
		if(b_size_temp==8){
			tot_time_new  = tot_time_new / no_threads_temp;
			float latency = (float)(tot_time_new*b_size_temp*1000)/(1024*1024*1024*sizeof(char));
			printf("Latency Is: %f\n", latency);
		}
		else{
			tot_time_new  = tot_time_new / no_threads_temp;
			float th_new = (1024*1024*1024*sizeof(char)/tot_time_new)/(1024*1024);			
			printf("Throughput is: %f\n", th_new);
		}		
	}
	return 0;
}