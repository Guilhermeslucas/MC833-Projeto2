#import <stdio.h>
#import <stdlib.h>
#import <math.h>
#import "colisionChecker.h"

/// Evaluate how many time the given car will take to arrive to the center
/// Returns -INFINITY if the car isn't moving or a negative value if it 
/// already crossed the center
double timeToCenter(Car car) {
	double time = 0;
	int speed = car.speed;
	int position = car.position;

	if (speed == 0) time = -INFINITY; 
	else time = -(position / speed); 

	// TODO: Add the sime since the last status
	if (time > 0) {
		// time += car.timestamp
	}

	return time;
}

/// Evaluate how many time the given car takes to cross the center
/// Returns -INFINITY if the car isn't moving
double timeToCrossCenter(Car car) {

	double time = 0;
	int speed = car.speed;

	if (speed == 0) time = -INFINITY; 
	else time = car.size / abs(speed); 

	return time;
}

ColisionType checkColision(Car car1, Car car2) {

	double time1 = timeToCenter(car1);
	double time2 = timeToCenter(car2);
	double timeSize1 = timeToCrossCenter(car1);
	double timeSize2 = timeToCrossCenter(car2);

	// Check if the cars aren't on the same direction
	if (car1.direction != car2.direction) {
		if (time1 + timeSize2 < 0 || time2 + timeSize2 < 0) return noColision;

		// Check if both cars will arive at the same time or if one car
		// will arrive while the other is still crossing the center
		if ((time1 == time2) ||
			(time1 > time2 && time1 <= time2 + timeSize2) ||
			(time2 > time1 && time2 <= time1 + timeSize1)) {
			
			// Check if a colision will occur
			if ((time1 >= 0 && time1 <= timeSize2) || 
				(time2 >= 0 && time2 <= timeSize1)) {
				
				return colision;
			}

			return possibleColision;
		}
	}

	return noColision;
}

