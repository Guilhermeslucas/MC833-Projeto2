#ifndef definitions_h
#define definitions_h

/// Boolean 
typedef enum Bool {
	False = 0,
	True = 1
} Bool;

/// The type of the message
typedef enum messageType {
	other = 0,
	security = 1,
	entertainment = 2,
	confortTraffic = 3
} MessageType;

/// The direction of the car
typedef enum Direction {
	horizontal = 0,
	vertical = 1
} Direction;

/// Define the possible actions that a car can make
typedef enum Action {
	none = 0,
	brake = 1,
	accelerate = 2,
	ambulance = 3,
} Action;

/// Define the possible situations when two cars can colide
typedef enum ColisionType {
	noColision = 0,
	possibleColision = 1,
	colision = 2
} ColisionType;

/// Definition of the message struct
typedef struct message {
	MessageType type; 
	int size;
	int speed;
	int position;
	int timestamp;
	char message[100];
	Direction direction;
} Message;

typedef struct serverMessage {
	MessageType type;
	Action action;
	char message[100];
} ServerMessage;

/// Definition of the car struct
typedef struct car {
	int id;
	int size;
	int speed;
	int position;
	int timestamp;
	Direction direction;
} Car;

typedef struct IndexMessage {
	int id;
	Message message;
} IndexMessage;

#endif
