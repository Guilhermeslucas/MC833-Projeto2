#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

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
int *client_socket, max_clients = 50;

/// Check if will happen a colision between two cars and set the cars ids
ColisionType checkCars(int *car1, int *car2, int size);

void *sendMessage(void *ptr);

int main(int argc, char *argv[]) {
    struct sockaddr_in server, client;
    Message buffer;
    int sockfd, addrlen, new_socket, activity, i, valread , sd;
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

            if (FD_ISSET(sd, &readfds)) {
                if ((valread = read(sd, (char *) &buffer, sizeof(buffer))) == 0) {
                    close(sd);
                    client_socket[i] = 0;
                } else {
					pthread_t messageThread;

					IndexMessage indexMessage;

					indexMessage.id = i;
					indexMessage.message = buffer;

					pthread_create(&messageThread, NULL, sendMessage, &indexMessage);
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

void *sendMessage(void *ptr) {

	IndexMessage *indexMessage = (IndexMessage *) ptr;
	int id = indexMessage->id;
	Message message = indexMessage->message;

	// Check the type of the message and simulate the delay
	if (message.type == security) {

		Car car = cars[id];
		
		car.id = id;
		car.size = message.size;
		car.speed = message.speed;
		car.position = message.position;
		car.timestamp = message.timestamp;
		car.direction = message.direction;

		int car1 = -1, car2 = -1;

		ColisionType colisionType = checkCars(&car1, &car2, max_clients);

		sleep(10);

		ServerMessage response1, response2;

		switch (colisionType) {
			case possibleColision:
				// Send a message to the first car stop
				response1.type = security;
				response1.action = brake;
				send(client_socket[car1], (char *) &response1, sizeof(response1), 0);
				
				// Send a message to the second car accelerate
				response2.type = security;
				response2.action = accelerate;
				send(client_socket[car2], (char *) &response2, sizeof(response2), 0);

				break;
			case colision:
				// Send a message to call an ambulance
				response1.type = security;
				response1.action = ambulance;

				send(client_socket[car1], (char *) &response1, sizeof(response1), 0);
				send(client_socket[car2], (char *) &response1, sizeof(response1), 0);
				
				break;
			default:
				break;
		}
	} else {
		sleep(100);

		// Just echo the received message
		ServerMessage response;
		strcpy(response.message, message.message);
		send(client_socket[id], (char *) &response, sizeof(response), 0);
	}

	return NULL;
}


