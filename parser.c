#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define DELIMITER ','

void decodeMessage(char *message) {

	char *tmp = message;
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

		char *slice = strtok(message, delim);
		int index = 0;

		while (slice) {

			// Check if isn't out of the size fomr the array
			if (!(index < nOfElements)) break;

			strcpy(infos[index], slice);
			slice = strtok(NULL, delim);
		}
	}
	
	//TODO: Transform the infos into a message 'object'
}