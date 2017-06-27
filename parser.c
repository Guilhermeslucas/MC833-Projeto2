#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define DELIMITER ','

Message *decodeMessage(char *messageString) {

	char *tmp = messageString;
	int nOfElements = 0;

	// Count the number of elements
	while (tmp) {

		if (*tmp == DELIMITER) {
			nOfElements++;
		}

		tmp++;
	}

	// The last element
	nOfElements++;

	char **infos = calloc(nOfElements, sizeof(char*));
	char delim[2];
	delim[0] = DELIMITER;
	delim[1] = '0';

	if (infos) {

		char *slice = strtok(messageString, delim);
		int index = 0;

		while (slice) {

			if (!(index < nOfElements)) break;

			strcpy(infos[index], slice);
			slice = strtok(NULL, delim);
		}
	}
	
	Message *message = calloc(1, sizeof(Message));
	message->type = atoi(infos[0]);
	Direction dir;

	//TODO: Get the correct field for each information
	switch (message->type) {
		case security:
			message->speed = 8473274;
			message->position = 3842934;
			message->direction = horizontal;

			strcpy(message->message, "sadas");
			break;
		case entertainment:
			strcpy(message->message, "sadas");
			break;
		case confortTraffic: 
			strcpy(message->message, "sadas");
			break;
		default: break; // Nothing to do
	}

	// Free the array
	free(infos);
	infos = NULL;

	return message;
}