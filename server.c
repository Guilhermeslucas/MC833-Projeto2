#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "parser.h"
#include "definitions.h"

#define LISTEN_PORT 12346
#define MAX_PENDING 5
#define BUFFER_SIZE 256

int main() {
    struct sockaddr_in server, client;
    char buf[BUFFER_SIZE];
    int sockfd, err, client_fd;
    pid_t pid;
    char client_ip[INET_ADDRSTRLEN];

	// Initializing the server
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

    while (1) {

        socklen_t client_len = sizeof(client);
        client_fd = accept(sockfd, (struct sockaddr *) &client, &client_len);

        inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip));

        if ((pid = fork()) == 0) {

            close(sockfd);

            if (client_fd < 0) {
                perror("Could not establish new connection\n");
                exit(1);
            }

            while (1) {

                int read = recv(client_fd, buf, BUFFER_SIZE, 0);

                if (!read) {
                    close(client_fd);
                    exit(0);
                }

                if (read < 0){
                    perror("Client read failed\n");
                    exit(1);
                }

				// Decode the received message
				Message *message = decodeMessage(buf);

				// Check the type of the message and simulate the delay
				if (message->type == security) {
					sleep(10);
				} else {
					sleep(100);
				}

                err = send(client_fd, buf, read, 0);
                if (err < 0) {
                    perror("Client write failed\n");
                    exit(1);
                }
            }
        }
        close(client_fd);
  }
}