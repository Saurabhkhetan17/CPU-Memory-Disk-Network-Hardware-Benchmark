#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>
#include<sys/time.h>
#include<malloc.h>
#include <pthread.h>

#define data 65000
#define total_num 8388608 / 64

char *srvr_mem;
char *cl_mem;
int src_cl;
int total_sz;
int total_th;
struct sockaddr_in udp_serv;

void *connection_handler(void *socket_desc)
{
  int i,udp_send,udp_rcv;
  for(i=0;i<total_num/total_th;i++)
  {
  udp_send = sendto(src_cl, srvr_mem, strlen(srvr_mem) , 0 , (struct sockaddr *) &udp_serv, total_sz);
  if (udp_send < 0)
      {
        printf("Sending Failed");
          exit(1);
      }
      udp_rcv = recvfrom(src_cl, cl_mem, data, 0, (struct sockaddr *) &udp_serv, &total_sz);
      if ( udp_rcv < 0)
      {
          printf("Receive Failed");
          exit(1);
        }
  }
  return 0;
}


int main(int argc, char *argv[])
{

    int i;
    total_sz = sizeof(udp_serv);
    srvr_mem = (char *) malloc(data);
    cl_mem = (char *) malloc(data);
    clock_t start, end;
    float total_time;
    //scanf("%d", &total_th);
    total_th = atoi(argv[1]);
    pthread_t thrd[total_th];
    double lat;
    double thrp;
    double thrput;
    int port = 1600;

    src_cl=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if ( src_cl < 0)
    {
        printf("Socket Creation Failed");
        exit(1);
    }

    memset((char *) &udp_serv, 0, sizeof(udp_serv));
    udp_serv.sin_family = AF_INET;
    udp_serv.sin_port = htons( port );
    memset(srvr_mem,'$',data);

    if (inet_aton("127.0.0.1" , &udp_serv.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    if(1)
    {
        start = clock();
        for(i=0;i<total_th;i++)
        {
              pthread_create(&thrd[i],NULL,connection_handler,NULL);
       }
      for(i=0;i<total_th;i++)
       {
              pthread_join(thrd[i],NULL);
       }
      end = clock();
      total_time = ((float) (end - start)) / CLOCKS_PER_SEC;
      lat = ((double)(total_time * 1000)) / total_num;
      printf("latancy:%f\n",lat);
      thrp = ((double)(64 * total_num))/total_time;
      thrput = ((double)(thrp * 8)) / 1024;
      printf("throuhputghput(Mb/sec):%f\n",thrput);

  }

  close(src_cl);
  return 0;
}
