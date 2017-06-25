
// The type of the message
typedef enum messageType {
	other = 0,
	security = 1,
	entertainment = 2,
	confortTraffic = 3
} MessageType;

// Definition of a point, with x and y coordinates
typedef struct point {
	int x;
	int y;
} Point;

// Definition of the message struct
typedef struct message {
	messageType type; 
	int speed;
	point position;
	char *message;
} Message;