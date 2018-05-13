#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define data 65000


int main(void)
{
    struct sockaddr_in client, server;

    int udp_serv;
    int i;
    int data_r;
    int srvr_b;
    int serv_s;
    int srvr_sz = sizeof(server);
    char *cl_mem=(char *) malloc(data);
    int port = 1600;

    udp_serv=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_serv < 0)
    {
        printf("Socket Creation Failed");
        exit(1);
    }

    memset((char *) &client, 0, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_port = htons(port);
    client.sin_addr.s_addr = htonl(INADDR_ANY);

        srvr_b = bind(udp_serv , (struct sockaddr*)&client, sizeof(client) );
        if( srvr_b < 0)
        {
          printf("Binding Failed");
          exit(1);
        }

        while(1)
        {
            printf("Waiting for Connection\n");
            fflush(stdout);

            data_r = recvfrom(udp_serv, cl_mem, data, 0, (struct sockaddr *) &server, &srvr_sz);
            if (data_r < 0)
            {
                printf("Receive Failed");
                exit(1);
            }
            serv_s = sendto(udp_serv, cl_mem, strlen(cl_mem), 0, (struct sockaddr*) &server, srvr_sz);
               if ( serv_s < 0)
               {
                     printf("Send Failed");
                   exit(1);
               }
           }

           close(udp_serv);
           return 0;
       }
