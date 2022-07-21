#include <iostream>	
#include <stdlib.h>
#include <pigpio.h>
#include "Joint.h"

Joint::Joint(int pin) : pin(pin){}

int Joint::AngleToPosition(int angle){
	return ((angle / 270.0) * 2000) + 500;
}



std::vector<float> Joint::LinearInterpolate2(int steps, float speed, float time){
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
	std::cout << "Time in pauses: " << elapsedTime * 2 << " Size: " << ret.size() << " Steps: " << steps << " Count: " << count << std::endl;
	return ret;
	
}

std::vector<float> Joint::LinearInterpolate(int steps, float speed, float time){
	float a = 0.01 * steps;
	float y1 = 1;
	float y2 = speed;
	float x1 = 0.0;
	float x2 = a;
	
	std::cout << "INTERPOLATION" << std::endl;
	//std::cout << "A: " << a << std::endl;
	//std::cout << "Given speed: " << speed << std::endl;
	
	
	std::vector<float> ret;
	float sum = 0;
	float s = 0;
	int c = 0;
	
	std::cout << "Given speed: " << speed << std::endl;
	
	while (s < speed && c < steps / 2){
		s = y1 + ((y2 - y1) / (x2 - x1)) * (c - x1);
		float p = 0.135 / s;
		sum += p;
		ret.push_back(p);
		c++;
		std::cout << "Pause: " << p << " Speed: " << s << std::endl;
	}
	
	std::cout << "C: " << c << std::endl;
	
	/*for (int i = 0; i < a; i++){
		float s = y1 + ((y2 - y1) / (x2 - x1)) * (i - x1);
		float p = 0.135 / s;
		sum += p;
		ret.push_back(p);
	}*/
	
	float p = (time - sum * 2) / (steps - c * 2);
	//std::cout << "New speed: " << 0.135 / p << std::endl;
	for (int i = 0; i < steps - 2 * a; i++){
		ret.push_back(p);
	}
	
	for (int i = 1; i < c + 1; i++){
		ret.push_back(ret[c - i]);
	}
	
	//std::cout << "Steps: " << steps << std::endl;
	//std::cout << "Len: " << ret.size() << std::endl;
	//std::cout << "Total in pause: " << sum << std::endl;
	return ret;
}

void Joint::SetAngle(int angle, float time){
	int desiredPosition = AngleToPosition(angle);
	int diff = desiredPosition - position;
	float timePerStep = float(time) / float(abs(diff));
	float accelerationStepsNum = abs(diff) * accelerationTimePercentage;
	float firstStepTime = timePerStep * accelerationPercentage;
	float accelerationStep = (firstStepTime - timePerStep) / accelerationStepsNum;
	
	
	std::vector<float> pauses = LinearInterpolate2(abs(diff), abs(diff) * 0.135 * (1 / time), time);
	
	int newPosition = position;
	double start = time_time();
	
	for (int i = 0; i < abs(diff) + 1; i++){
		newPosition += diff / abs(diff);
		
		gpioServo(pin, newPosition);
		time_sleep(pauses[i]);
	}
	std::cout << "Elapsed time: " << time_time() - start << std::endl;
	
	position = desiredPosition;
}

int Joint::GetPin() const{
	return pin;
}

int Joint::GetPosition() const{
	return position;
}
