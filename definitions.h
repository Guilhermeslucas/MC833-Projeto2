#ifndef definitions_h
#define definitions_h

#import <time.h>

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
typedef enum CollisionType {
	noCollision = 0,
	possibleCollision = 1,
	collision = 2
} CollisionType;

/// Definition of the client message struct
typedef struct clientMessage {
	MessageType type; 
    int id;
	int size;
	int speed;
	int position;
	time_t timestamp;
	char message[100];
	Direction direction;
} ClientMessage;

/// Definition of the server message struct
typedef struct serverMessage {
    int id;
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

/// Auxiliary struct to pass an id and a message to new thread
typedef struct IndexMessage {
	int id;
	ClientMessage message;
} IndexMessage;

#endif
