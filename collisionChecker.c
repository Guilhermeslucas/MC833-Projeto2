#import <stdio.h>
#import <stdlib.h>
#import <math.h>
#include <time.h>
#import "collisionChecker.h"

/// Evaluate how many time the given car will take to arrive to the center
/// Returns -INFINITY if the car isn't moving or a negative value if it 
/// already crossed the center
double timeToCenter(Car car) {
	double timeToC = 0;
	int speed = car.speed;
	int position = car.position;

	if (speed == 0) timeToC = -INFINITY; 
	else timeToC = -(position / speed); 

	if (timeToC > 0) {
		int timestamp = time(NULL);
		timeToC -= timestamp - car.timestamp;
	}

	return timeToC;
}

/// Evaluate how many time the given car takes to cross the center
/// Returns -INFINITY if the car isn't moving
double timeToCrossCenter(Car car) {

	double timeToC = 0;
	int speed = car.speed;

	if (speed == 0) timeToC = -INFINITY; 
	else timeToC = car.size / abs(speed); 

	return timeToC;
}

CollisionType checkCollision(Car car1, Car car2) {

	double time1 = timeToCenter(car1);
	double time2 = timeToCenter(car2);
	double timeSize1 = timeToCrossCenter(car1);
	double timeSize2 = timeToCrossCenter(car2);

	// Check if the cars aren't on the same direction
	if (car1.direction != car2.direction) {
		if (time1 + timeSize2 < 0 || time2 + timeSize2 < 0) return noCollision;

		// Check if both cars will arive at the same time or if one car
		// will arrive while the other is still crossing the center
		if ((time1 == time2) ||
			(time1 > time2 && time1 <= time2 + timeSize2) ||
			(time2 > time1 && time2 <= time1 + timeSize1)) {
			
			// Check if a collision will occur
			if ((time1 >= 0 && time1 <= timeSize2) || 
				(time2 >= 0 && time2 <= timeSize1)) {
				
				return collision;
			}

			return possibleCollision;
		}
	}

	return noCollision;
}

