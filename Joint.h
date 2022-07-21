#ifndef JOINT
#define JOINT

#include <iostream>
#include <vector>
#include <thread>

class Joint{
	private:
		int pin;
		int position = 1500;
		float accelerationTimePercentage = 0.05;
		float accelerationPercentage = 10;
		
	public:
		// Constructors
		Joint(int pin);
		
		// Getters
		int GetPin() const;
		int GetPosition() const;
		
		// Functions
		void SetAngle(int angle, float time);
		int AngleToPosition(int angle);
		std::vector<float> LinearInterpolate(int steps, float speed, float time);
		std::vector<float> LinearInterpolate2(int steps, float speed, float time);
};

#endif 
