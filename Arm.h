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
		struct Point{
			float x;
			float y;
			float z;
			
			void Print(){
				std::cout << "X: " << x << " Y: " << y << " Z: " << z << std::endl;
			}
			//Point(float x, float y, float z) : x(x), y(y), z(z) {}
		};
		
		Point endEffector = {0.1, 28.6, 0.1};
		// Constructors
		Arm();
		Arm(std::vector<Joint> joints);
		
		// Setters
		void AddJoints(std::vector<Joint> joints);
		
		// Getters
		std::vector<Joint> GetJoints() const;
		Joint GetJoint(int index) const;
		Point GetEndEffector() const;
		
		// Functions
		void AddJoint(Joint joint);
		void SetAngle(Joint *joint, int angle, float time);
		void JoinThreads();
		void SetPosition(Point p, float time);
		void Interpolate(Point p, float time);
};

#endif
