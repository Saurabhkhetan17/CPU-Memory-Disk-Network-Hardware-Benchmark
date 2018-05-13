#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<malloc.h>

#define data 65536

void *thread_num_handler(void *server_socket)
{

  int tcp_server = *(int*)server_socket;
  int mess;
  char *client_data=(char *) malloc(data);

  while( (mess = recv(tcp_server , client_data , data , 0)) > 0 )
  {

      write(tcp_server , client_data , strlen(client_data));
  }

  if(mess == 0)
  {
      puts("Client Disconnected..!!!!\n");
      fflush(stdout);
  }
  else if(mess == -1)
  {
      perror("Receiving Failed....!!!!!\n");
    }

        free(server_socket);

        return 0;
    }

    int main(int argc , char *argv[])
    {
        int server_socket , tcp_client , socket_s , *add_socket, mess;
        struct sockaddr_in server , client;
        char *client_data=(char *) malloc(data);
        int port = 1500;

        server_socket = socket(AF_INET , SOCK_STREAM , 0);
        if (server_socket == -1)
        {
            printf("Socket Down..!!!!!\n");
        }
        puts("Socket created...!!!!\n");

        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons( port );

         int   s_bind = bind(server_socket,(struct sockaddr *)&server , sizeof(server));
        if( s_bind == -1)
        {

                        perror("Bind Failed");
                        return 1;
                    }
                    puts("Bind Done");

                    listen(server_socket , 3);

                    puts("Waiting for Client Connections");
                    socket_s = sizeof(struct sockaddr_in);

                    while( (tcp_client = accept(server_socket, (struct sockaddr *)&client, (socklen_t*)&socket_s)) )
                    {
                        puts("Connection Accepted");

                        pthread_t thread_count;
                        add_socket = malloc(1);
                        *add_socket = tcp_client;

                        if( pthread_create( &thread_count , NULL ,  thread_num_handler , (void*) add_socket) < 0)
                        {
                            perror("Couldn't create Thread..!!!\n");
                            return 1;
                        }

                        puts("Connected to new client\n");
                    }

                    if (tcp_client < 0)
                    {
             perror("Could'nt Accept..!!!!\n");
             return 1;
 }

 return 0;
}