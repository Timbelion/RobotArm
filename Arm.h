#ifndef ARM
#define ARM

#include <iostream>
#include <vector>
#include <thread>
#include "Joint.h"

class Arm{
	private:
		std::vector<std::thread> waitingThreads;
		
	public:
		std::vector<Joint> joints;
	
		// Constructors
		Arm();
		Arm(std::vector<Joint> joints);
		
		// Setters
		void AddJoints(std::vector<Joint> joints);
		
		// Getters
		std::vector<Joint> GetJoints() const;
		Joint GetJoint(int index) const;
		
		// Functions
		void AddJoint(Joint joint);
		void SetAngle(Joint *joint, int angle, float time);
		void JoinThreads();
};

#endif
