
// The type of the message
typedef enum messageType {
	other = 0,
	security = 1,
	entertainment = 2,
	confortTraffic = 3
} MessageType;

// The direction of the car
typedef enum Direction {
	horizontal = 0,
	vertical = 1
} Direction;

// Define the possible actions that a car can make
typedef enum Action {
	none = 0,
	brake = 1,
	accelerate = 2,
	ambulance = 3,
} Action;

// Definition of a point or a vector
typedef struct point {
	int x;
	int y;
} Point, Vector;

// Definition of the message struct
typedef struct message {
	messageType type; 
	Vector speed;
	Point position;
	char *message;
} Message;

// Definition of the car struct
typedef struct car {
	int id;
	Vector speed;
	Point position;
} Car;
