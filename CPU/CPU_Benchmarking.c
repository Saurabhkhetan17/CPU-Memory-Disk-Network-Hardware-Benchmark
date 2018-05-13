#include<stdio.h>
#include<sys/types.h>
#include<pthread.h>
#include<time.h>
#include<immintrin.h>
#include<unistd.h>
int n;
void *FLOPS_AVX()
{

 __m256 vec1 = _mm256_set_ps (2.2, 3.2, 4.6, 1.4, 0.8, 20.9, 1.1, 6.2);
 __m256 vec2 = _mm256_set_ps (2.2, 3.2, 4.6, 1.4, 0.8, 20.9, 1.1, 6.2);
 __m256 res_mul;
 __m256 res_add;

 int g=0;
 for(g = 0; g<1e9/n; g++)
 {
  res_mul = _mm256_mul_ps(vec1,vec2);
  res_add = _mm256_add_ps(vec1,vec2);
 }
}


void *IOPS_AVX()
{

 __m256i vec1 = _mm256_setr_epi32(2,1,2,3,4,5,6,7);
 __m256i vec2 = _mm256_setr_epi32(2,4,6,8,1,3,4,5);
 __m256i res_mul;
 __m256i res_add;

 int g=0;
 for(g = 0; g<1e9/n; g++)
 {
  res_mul = _mm256_mul_epi32(vec1,vec2);
  res_add = _mm256_add_epi32(vec1,vec2);
 }
}


void *FLOPS()
{
float p = 10.00333333;
float o = 1.5;
float sum = 0;
int z = 0;
int m = 0;
float sub =0;


for(z = 0; z<1e9/n; z++)
 {
 sum = p + o;
 sub = p - o;
 p = p+p;
 o = o+o;
sum = p + o;
 sub = p - o;

 p = p+p;
 o = o+o;
sum = p + o;
 sub = p - o;

 p = p+p;
 o = o+o;
sum = p + o;
 sub = p - o;

 p = p+p;
 o = o+o;
 }
}


void *IOPS()
{
int p = 10;
int o = 1;
int x = 0;
int z=0;
int sub =0;
int mul =1;
int div = 0;

for(z = 0; z<1e9/n; z++)
 {
    x=p+o;
    sub = p - o;
    p = p+p;
    o = o + o;
    
     x=p+o;
    sub = p - o;
    p = p+p;
    o = o + o;
    x=p+o;
    sub = p - o;
    p = p+p;
    o = o + o;
     x=p+o;
    sub = p - o;
    p = p+p;
    o = o + o;
 }
}


int main(int argc, char *argv[])
{

 clock_t start;
 clock_t end;
 int a=0, b=0;
 int ops;
 pthread_t new_thread[n];
 n = atoi(argv[1]);

 ops = atoi(argv[2]);
  if(ops == 1)
  {
  start = clock();
  for(a=0; a<n; a++)
         {
           pthread_create(&new_thread[a], NULL, &IOPS, NULL);
         }

         for(b=0; b<n; b++)
         {
          pthread_join(new_thread[b],NULL);
         }
    end = clock();
  double GIOPS;
  double total_time = ((double)(end - start))/CLOCKS_PER_SEC;
  printf("function took %f seconds to execute \n", total_time);
  double  IOPS= (double) ((1e9 * 17 * n)/total_time);

  GIOPS = (double) IOPS/1e9;
  printf("GIOPS %lf \n",GIOPS);
  }

 else if(ops == 2)
{
 start = clock();
 for(a=0; a<n; a++)
         {
           pthread_create(&new_thread[a], NULL, &FLOPS, NULL);
         }

         for(b=0; b<n; b++)
         {
          pthread_join(new_thread[b],NULL);
         }
 end = clock();
  double GFLOPS;
  double total_time = ((double)(end -start))/CLOCKS_PER_SEC;
  printf("function took %f seconds to execute \n", total_time);
 double FLOPS= (double) ((1e9 * 17 * n)/total_time);

  GFLOPS = (double) FLOPS/1e9;
  printf("GFLOPS %lf \n",GFLOPS);

}

 else if(ops == 3)
 {
  start = clock();
 for(a=0; a<n; a++)
         {
           pthread_create(&new_thread[a], NULL, &IOPS_AVX, NULL);
         }

         for(b=0; b<n; b++)
         {
          pthread_join(new_thread[b],NULL);
          }
   end = clock();
  double GIOPS;
  double total_time = ((double)(end -start))/CLOCKS_PER_SEC;
  printf("function took %f seconds to execute \n", total_time);
  double IOPS= (double) ((1e9 * 16 * n)/total_time);

  GIOPS = (double) IOPS/1e9;
  printf("GIOPS with AVX %lf \n",GIOPS);

}

 else if(ops == 4)
 {
  start = clock();
 for(a=0; a<n; a++)
         {
           pthread_create(&new_thread[a], NULL, &FLOPS_AVX, NULL);
         }

         for(b=0; b<n; b++)
         {
          pthread_join(new_thread[b],NULL);
          }
   end = clock();
  double GFLOPS;
  double total_time = ((double)(end -start))/CLOCKS_PER_SEC;
  printf("function took %f seconds to execute \n", total_time);
 double FLOPS= (double) ((1e9 * 16 * n )/total_time);

  GFLOPS = (double) FLOPS/1e9;
  printf("GFLOPS with AVX %lf \n",GFLOPS);

 }
return 0;
}