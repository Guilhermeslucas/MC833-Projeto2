#ifndef parser_h
#define parser_h

#include "definitions.h"

/// Parse the message string to a message
Message *decodeMessage(char *messageString);

/// Encode a message into a string
char encodeMessage(Message message);

#endif