#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include "definitions.h"

#define h_addr h_addr_list[0]
#define SERVER_PORT 12345
#define MAX_LINE 256

int main(int argc, char * argv[])
{
    struct hostent *host_address;
    struct sockaddr_in socket_address;
    char *host_name;
    char buf[MAX_LINE];
    int socket_fd;
    ClientMessage* message; 
    time_t timestamp_sec; 
    ServerMessage server_message;
    
    message = malloc(sizeof(ClientMessage));
    server_message = malloc(sizeof(ServerMessage));

    /* arguments verification*/
    if (argc != 8) {
        printf("You should pass arguments like this:\n");
        printf("type id size speed position direction message\n");
        exit(1);
    }

    /*create struct to send messsage*/
    message->type = argv[1];
    message->id = atoi(argv[2]);
    message->size = atoi(argv[3]);
    message->speed = atoi(argv[4]);
    message->position = atoi(argv[5]);
    message->timestamp = time(&timestamp_sec);
    message->direction = argv[6];
    strcpy(message->message,argv[7]);


    /* criacao de socket ativo*/
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        printf("Not able to create socket\n");
        exit(1);
    }

    /* traducao de nome para endereco IP */
    host_name = "localhost";
    host_address = gethostbyname(host_name);

    if (host_address == NULL) {
        printf("Not possible to resolve host name\n");
        exit(1);
    }
    
    /* criaÃ§Ã£o da estrutura de dados de endereÃ§o */
    bzero((char *)&socket_address, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(SERVER_PORT);
    bcopy((char *)host_address->h_addr, 
            (char *)&socket_address.sin_addr.s_addr, host_address->h_length);

    
    /* estabelecimento da conexÃ£o */
    if (connect(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0) {
        printf("Not able to connect\n");
        exit(1);
    }

    /* ler e enviar linhas de texto, receber eco */
    while(1) {

        if(send(socket_fd, message, sizeof(ClientMessage) , 0) < 0) {
            printf("Not possible to send the message\n");
            exit(1);
        }
        
       
        if (read(socket_fd, server_message, sizeof(ServerMessage)) < 0) {
            printf("Not possible to read from socket\n");
            exit(1);
        }
        
        if (server_message.type != noCollision) {
            if (server_message.action == brake || server_message.action == ambulance)
                message.speed = 0;
            
            else if (server.message.action == accelerate)
                message.speed = 10;
        }

        sleep(2);

    }
    
    close(socket_fd);
    return 0; 
}
