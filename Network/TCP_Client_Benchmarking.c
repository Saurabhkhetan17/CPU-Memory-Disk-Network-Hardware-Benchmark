#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/time.h>
#include<malloc.h>
#include <pthread.h>
#define data 65536
#define total_num 8388608 / 64

int header;
int total_th;
char *srvr_mem;
char *cl_mem;

void *thread_count(void *descr)
{
    int i,cl_send,cl_rcv;
    for(i=0;i<total_num/total_th;i++)
    {
        cl_send = send(header , srvr_mem , strlen(srvr_mem) , 0);
    if( cl_send < 0)
        {
            puts("Sending Failed....!!!!\n");
        }
        cl_rcv = recv(header , cl_mem , data , 0);
        if( cl_rcv < 0)
        {
            puts("Receiving Failed.....!!!!\n");
        }
    }


        return 0;
    }

    int main(int argc , char *argv[])
    {
        int i,c_conn;
        struct sockaddr_in server;
        clock_t start, end;
        double total_time,lat,thrput,thrp;
        //scanf("%d", &total_th);
        total_th = atoi(argv[1]);
        pthread_t num_thread[total_th];
        int port = 1500;
        srvr_mem = (char *) malloc(data);
        cl_mem = (char *) malloc(data);

        header = socket(AF_INET , SOCK_STREAM , 0);
        if (header < 0)
        {
            printf("Socket not Created....!!!!\n");
          }
          printf("Socket is UP...!!!!\n");

          server.sin_addr.s_addr = inet_addr("127.0.0.1");
          server.sin_family = AF_INET;
          server.sin_port = htons( port );

          memset(srvr_mem,'Z',data);
            c_conn = connect(header , (struct sockaddr *)&server , sizeof(server));
            if (c_conn == -1)
            {
                perror("No COnnection..!!!!\n");
                return 1;
            }

            printf("Connected....!!!!!\n");

            if(1)
            {
                start = clock();
                for(i=0;i<total_th;i++)
                 {
                        pthread_create(&num_thread[i],NULL,thread_count,NULL);
                 }
                for(i=0;i<total_th;i++)
                 {
                        pthread_join(num_thread[i],NULL);
                 }
                 end = clock();
                 total_time = ((float) (end - start)) / CLOCKS_PER_SEC;
                 lat = ((double)(total_time * 1000)) / total_num;
                 printf("latancy:%f\n",lat);
                 thrp = ((double)(64 * total_num))/total_time;
                 thrput = ((double)(thrp * 8)) / 1024;
                 printf("throuhputghput(Mb/sec):%f\n",thrput);
             }
             close(header);
         }