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

#include "definitions.h"
#include "collisionChecker.h"

#define LISTEN_PORT 12345
#define MAX_PENDING 5
#define BUFFER_SIZE 256

Car *cars;
int *client_socket, max_clients = 50, nClients = 0;
pthread_mutex_t lock;

/// Check if will happen a collision between a given car and the others and set the cars ids
CollisionType checkCars(int car1, int *car2, int size);

void *sendMessage(void *ptr);

int main(int argc, char *argv[]) {
    struct sockaddr_in server, client;
    ClientMessage buffer;
    int sockfd, addrlen, new_socket, activity, i, valread , sd;
    int max_sd;
    fd_set readfds;

	client_socket = calloc(max_clients, sizeof(int));
	cars = calloc(max_clients, sizeof(Car));

	// Initialize the server
    for (i = 0; i < max_clients; i++) {
        client_socket[i] = 0;
    }

	if (pthread_mutex_init(&lock, NULL) != 0)
    {
        perror("Mutex init failed");
        exit(1);
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

        activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);

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
					nClients++;
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
					nClients--;
                } else {
					pthread_t messageThread;

					IndexMessage indexMessage;

					indexMessage.id = i;
					indexMessage.message = buffer;

					printf("id: %d size: %d pos: %d speed: %d dir: %d\n", buffer.id, buffer.size, buffer.position, buffer.speed, buffer.direction);

					pthread_create(&messageThread, NULL, sendMessage, &indexMessage);
                }
            }
        }
  	}
}

CollisionType checkCars(int car1, int *car2, int size) {

    pthread_mutex_lock(&lock);

	for (int i = 0; i < size; i++) {

		// If is the same ca ror if the car isn't anymore on the array,
		// just continue the loop
		if (car1 == i || cars[i].id == -1) continue;

		CollisionType collisionType = checkCollision(cars[car1], cars[i]);

		if (collisionType) {
			*car2 = i;
		
			pthread_mutex_unlock(&lock);

			return collisionType;
		}
	}

    pthread_mutex_unlock(&lock);

	return noCollision;
}

void *sendMessage(void *ptr) {

	IndexMessage *indexMessage = (IndexMessage *) ptr;
	int id = indexMessage->id;
	ClientMessage message = indexMessage->message;

	// Check if a car already had collided but still sending messages
	// and just return if so
	if (id != -1 && cars[id].id == -1) return NULL;

	// Check the type of the message and simulate the delay
	if (message.type == security) {
		
		pthread_mutex_lock(&lock);
		
		cars[id].id = id;
		cars[id].size = message.size;
		cars[id].speed = message.speed;
		cars[id].position = message.position;
		cars[id].timestamp = message.timestamp;
		cars[id].direction = message.direction;

	    pthread_mutex_unlock(&lock);

		int car2 = -1;

		CollisionType collisionType = checkCars(id, &car2, nClients);

		struct timespec ts;
    	ts.tv_sec = 10 / 1000;
    	ts.tv_nsec = 0.01 * 1000000;
    	nanosleep(&ts, NULL);

		ServerMessage response1, response2;

		switch (collisionType) {
			case possibleCollision:
				printf("Possible colision between car %d and car %d\n", id, car2);

				// Send a message to the first car stop
				response1.type = security;
				response1.action = brake;
				response1.id = id;
				send(client_socket[id], (char *) &response1, sizeof(response1), 0);
				
				// Send a message to the second car accelerate
				response2.type = security;
				response2.action = accelerate;
				response2.id = car2;
				send(client_socket[car2], (char *) &response2, sizeof(response2), 0);

				break;
			case collision:
				printf("Colision between car %d and car %d\n", id, car2);

				cars[id].id = -1;
				cars[car2].id = -1;

				// Send a message to call an ambulance
				response1.type = security;
				response1.action = ambulance;
				response1.id = id;

				send(client_socket[id], (char *) &response1, sizeof(response1), 0);
				
				response2.type = security;
				response2.action = ambulance;
				response2.id = car2;
				send(client_socket[car2], (char *) &response1, sizeof(response2), 0);

				break;
			default: break;
		}

		// If the car will not colide with any other, just sent a response
		if (car2 == -1) {
			response1.type = security;
			response1.action = none;
			response1.id = id;

			send(client_socket[id], (char *) &response1, sizeof(response1), 0);
		}
	} else {
		struct timespec ts;
    	ts.tv_sec = 100 / 1000;
    	ts.tv_nsec = 0.1 * 1000000;
    	nanosleep(&ts, NULL);

		printf("Got this message from car %d: %s\n", id, message.message);

		// Just echo the received message
		ServerMessage response;
		strcpy(response.message, message.message);
		send(client_socket[id], (char *) &response, sizeof(response), 0);
	}

	return NULL;
}


