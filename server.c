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
#include "colisionChecker.h"

#define LISTEN_PORT 12346
#define MAX_PENDING 5
#define BUFFER_SIZE 256

Car *cars;

/// Check if will happen a colision between two cars and set the cars ids
ColisionType checkCars(int *car1, int *car2, int size);

int main() {
    struct sockaddr_in server, client;
    char buffer[1025];
    int sockfd, addrlen, new_socket, *client_socket, max_clients = 50, activity, i, valread , sd;
    int max_sd;
    fd_set readfds;

	client_socket = calloc(max_clients, sizeof(int));
	cars = calloc(max_clients, sizeof(Car));

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

        for (i = 0 ; i < max_clients ; i++) {

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

						Car car = cars[i];
						
						car.id = i;
						car.size = message->size;
						car.speed = message->speed;
						car.position = message->position;
						car.timestamp = message->timestamp;
						car.direction = message->direction;

						int car1 = -1, car2 = -1;

						ColisionType colisionType = checkCars(&car1, &car2, max_clients);

                        sleep(10);

						switch (colisionType) {
							case possibleColision:
								send(client_socket[car1], "1,1", strlen("1,1"), 0);
								send(client_socket[car2], "1,2", strlen("1,2"), 0);
								break;
							case colision:
								send(client_socket[car1], "1,3", strlen("1,3"), 0);
								send(client_socket[car2], "1,3", strlen("1,3"), 0);
								break;
							default:
								break;
						}
                    } else {
                        sleep(100);
						send(sd, message->message, strlen(message->message), 0);
					}

					free(message);
                    message = NULL;
                }
            }
        }
  	}
}

ColisionType checkCars(int *car1, int *car2, int size) {

	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {

			ColisionType colisionType = checkColision(cars[i], cars[j]);

			if (colisionType) {
				*car1 = i;
				*car2 = j;

				return colisionType;
			}
		}
	}

	return noColision;
}

