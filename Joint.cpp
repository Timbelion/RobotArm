#include <iostream>	
#include <stdlib.h>
#include <pigpio.h>
#include "Joint.h"

Joint::Joint(int pin) : pin(pin){}

int Joint::AngleToPosition(int angle){
	return ((angle / 270.0) * 2000) + 500;
}



std::vector<float> Joint::LinearInterpolate(int steps, float speed, float time){
	float startSpeed = 2;
	
	std::vector<float> ret;
	float elapsedTime = 0;
	float totalTime = 100000;
	float count = 0;
	
	while(totalTime > time){
		float pause = 0.135 / startSpeed;
		elapsedTime += pause;
		
		ret.push_back(pause);
		
		totalTime = (elapsedTime * 2) + (((steps - 2 * count) * 0.135) / startSpeed);
		startSpeed += 2;
		count++;
		
		if (count >= steps / 2){
			ret.clear();
			std::cout << "ERROR" << std::endl;
			return ret;		
		}
	}
	
	for (int i = 0; i < steps - 2 * count; i++){
		ret.push_back(ret[count - 1]);
	}
	
	for (int i = 1; i < count + 1; i++){
		ret.push_back(ret[count - i]);
	}
	
	return ret;
	
}

void Joint::SetAngle(int angle, float time){
	int desiredPosition = AngleToPosition(angle);
	int diff = desiredPosition - position;
	float timePerStep = float(time) / float(abs(diff));
	float accelerationStepsNum = abs(diff) * accelerationTimePercentage;
	float firstStepTime = timePerStep * accelerationPercentage;
	float accelerationStep = (firstStepTime - timePerStep) / accelerationStepsNum;
	
	if (diff == 0)
		return;
	
	//std::vector<float> pauses = LinearInterpolate(abs(diff), abs(diff) * 0.135 * (1 / time), time);
	
	int newPosition = position;
	double start = time_time();
	
	//std::cout << "DIFF: " << abs(diff) << " TIME PER STEP: " << timePerStep << std::endl;
	
	for (int i = 0; i < abs(diff) + 1; i++){
		newPosition += diff / abs(diff);
		
		gpioServo(pin, newPosition);
		time_sleep(timePerStep);
	}
	//std::cout << "Elapsed time: " << time_time() - start << std::endl;
	
	position = desiredPosition;
}

int Joint::GetPin() const{
	return pin;
}

int Joint::GetPosition() const{
	return position;
}
