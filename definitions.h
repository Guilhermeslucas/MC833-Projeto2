
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

typedef struct car {
	int id;
	Vector speed;
	Point position;
} Car;
