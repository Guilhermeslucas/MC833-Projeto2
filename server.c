#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#include "parser.h"
#include "definitions.h"

#define LISTEN_PORT 12346
#define MAX_PENDING 5
#define BUFFER_SIZE 256

Car *cars;

int main() {
    struct sockaddr_in server, client;
    char buffer[1025];
    int sockfd, addrlen, new_socket, client_socket[30], max_clients = 30, activity, i, valread , sd;
    int max_sd;
    fd_set readfds;


	cars = calloc(50, sizeof(Car));

	// Initialize the server
    for (i = 0; i < max_clients; i++) {
        client_socket[i] = 0;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char *)&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(LISTEN_PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    listen(sockfd, MAX_PENDING);

    addrlen = sizeof(client);

    while (1) {

        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        max_sd = sockfd;

        for ( i = 0 ; i < max_clients ; i++) {

            sd = client_socket[i];

            if (sd > 0) {
                FD_SET( sd , &readfds);
            }

            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if (activity < 0 && errno != EINTR) {
            printf("select error");
        }

        if (FD_ISSET(sockfd, &readfds)) {

            if ((new_socket = accept(sockfd, (struct sockaddr *)&client, (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("Client: %s:%d\n" , inet_ntoa(client.sin_addr) , ntohs(client.sin_port));

            for (i = 0; i < max_clients; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    break;
                }
            }
        }

        for (i = 0; i < max_clients; i++) {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds)) {
                if ((valread = read(sd , buffer, 1024)) == 0) {
                    close(sd);
                    client_socket[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    
                    // Decode the received message
                    Message *message = decodeMessage(buffer);

                    // Check the type of the message and simulate the delay
                    if (message->type == security) {
                        sleep(10);
                    } else {
                        sleep(100);
                    }

                    free(message);
                    message = NULL;

                    send(sd, buffer, strlen(buffer), 0);
                }
            }
        }
  }
}
