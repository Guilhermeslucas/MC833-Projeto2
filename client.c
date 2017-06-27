#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
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

    /* verificaÃ§Ã£o de argumentos */
    if (argc != 8) {
        printf("You should pass arguments like this:\n");
        printf("type id size speed position direction message\n");
        exit(1);
    } 

    
    /* criaÃ§Ã£o de socket ativo*/
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        printf("Not able to create socket\n");
        exit(1);
    }

    /* traduÃ§Ã£o de nome para endereÃ§o IP */
    host_name = argv[1];
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
        printf("Please enter message: ");
        bzero(buf, MAX_LINE);
        fgets(buf, MAX_LINE, stdin);

        if(send(socket_fd, buf, strlen(buf) , 0) < 0) {
            printf("Not possible to send the message\n");
            exit(1);
        }
        
        bzero(buf, MAX_LINE);
       
        if (read(socket_fd, buf, MAX_LINE) < 0) {
            printf("Not possible to read from socket\n");
            exit(1);
        }
        printf("Message from server: %s", buf);

    }
    
    close(socket_fd);
    return 0; 
}
