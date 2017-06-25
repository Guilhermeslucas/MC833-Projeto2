#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define DELIMITER ','

Message *decodeMessage(char *messageString) {

	char *tmp = messageString;
	int nOfElements = 0;

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

			// Check if isn't out of the size fomr the array
			if (!(index < nOfElements)) break;

			strcpy(infos[index], slice);
			slice = strtok(NULL, delim);
		}
	}
	
	MessageType type = atoi(infos[0]);
	Message *message = calloc(1, sizeof(Message));
	
	//TODO: Get the correct field for each information
	switch (type) {
		case security:

			break;
		case entertainment:

			break;
		case confortTraffic: 

			break;
		default:
			break;
	}

	return message;
}