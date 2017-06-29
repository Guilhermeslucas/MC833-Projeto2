#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include <math.h>
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
    ServerMessage* server_message;
    int colision_flag = 0;
    int baseSpeed;
    
    message = malloc(sizeof(ClientMessage));
    server_message = malloc(sizeof(ServerMessage));

    /* arguments verification*/
    if (argc != 7) {
        printf("You should pass arguments like this:\n");
        printf("type size speed position direction message\n");
        exit(1);
    }

    /*create struct to send messsage*/
    message->type = atoi(argv[1]);
    message->id = -1;
    message->size = atoi(argv[2]);
    message->speed = atoi(argv[3]);
    message->position = atoi(argv[4]);
    message->direction = atoi(argv[5]);
    strcpy(message->message,argv[6]);

    baseSpeed = message->speed;

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
        message->timestamp = time(&timestamp_sec);
        message->position = message->position + message->speed;

        // Stop the simulation if the car is exiting the board
        if (abs(message->position) > 50) break;

        if(send(socket_fd, message, sizeof(ClientMessage) , 0) < 0) {
            printf("Not possible to send the message\n");
            exit(1);
        }
       
        if (read(socket_fd, server_message, sizeof(ServerMessage)) < 0) {
            printf("Not possible to read from socket\n");
            exit(1);
        }

        message->id = server_message->id;
        
        if (colision_flag) {
            colision_flag = 0;
            message->speed = baseSpeed;
        }

        if (server_message->type == security) {
            // If a colision happens, remove the car from the simulation
            if (server_message->action == ambulance) break;
            else if (server_message->action == brake) {
                message->speed = 0;
                colision_flag++;
            }
            
            else if (server_message->action == accelerate) {
                // Set the speed to the maximum based on the sign of the base speed
                message->speed = baseSpeed > 0 ? 10 : -10;

                colision_flag++;
            }
        }

        sleep(2);

    }
    
    close(socket_fd);
    return 0; 
}
