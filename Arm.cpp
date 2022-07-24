#include <iostream>	
#include <stdlib.h>
#include <pigpio.h>
#include <math.h>
#include "Arm.h"

Arm::Arm(){}

Arm::Arm(std::vector<Joint> joints) : joints(joints) {}

void Arm::AddJoints(std::vector<Joint> joints){
	this->joints = joints;
}

std::vector<Joint> Arm::GetJoints() const {
	return joints;
}

Joint Arm::GetJoint(int index) const {
	return joints[index];
}

Arm::Point Arm::GetEndEffector() const{
	return endEffector;
}

void Arm::AddJoint(Joint joint){
	joints.push_back(joint);
}

void Arm::SetAngle(Joint *joint, int angle, float time){
	waitingThreads.push_back(std::thread(&Joint::SetAngle, joint, angle, time));
}

void Arm::JoinThreads(){
	for (unsigned int i = 0; i < waitingThreads.size(); i++){
		waitingThreads[i].join();
	}
	waitingThreads.clear();
}

void Arm::SetPosition(Point p, float time){
	//std::cout << "Set pos X: " << p.x << " Y: " << p.y << " Z: " << p.z << std::endl;
	float angle0 = atan(p.x / p.z) * 180 / 3.1415;
	float armLength = 14.5;
	
	if ((p.x >= 0 && p.z >= 0) || (p.x <= 0 && p.z >= 0)){
		angle0 += 45;
	}
	else if ((p.x >= 0 && p.z <= 0) || (p.x <= 0 && p.z <= 0)){
		angle0 += 225;
	}
	
	float distanceHelp = sqrt(p.x * p.x + p.z * p.z);
	float distance = sqrt(distanceHelp * distanceHelp + p.y * p.y);
	float halfDistance = distance / 2;
	
	float angle1 = acos(halfDistance / armLength) * 180 / 3.1415;
	float angle2 = 180 - 2 * angle1;
	
	angle1 += 90 - (atan(distanceHelp / p.y) * 180 / 3.1415);
	
	angle1 = 90 - angle1 + 135;
	angle2 = 180 - angle2 + 135;
	
	//std::cout << "Angle0: " << angle0 << std::endl;
	//std::cout << "Angle1: " << angle1 << std::endl;
	//std::cout << "Angle2: " << angle2 << std::endl;
	//std::cout << "Distance: " << distance << std::endl;
	
	if (angle0 >= 0 && !isnan(angle0) && !isnan(angle1) && !isnan(angle2)){
		endEffector = p;
		this->SetAngle(&this->joints[0], angle0, time);
		this->SetAngle(&this->joints[1], angle1, time);
		this->SetAngle(&this->joints[2], angle2, time);
		this->JoinThreads();
	}
	else 
		std::cout << "Angle is negative: " << angle0 << " " << angle1 << " " << angle2 << std::endl;
}

void Arm::Interpolate(Point p, float time){
	
	Point startPosition = endEffector;
	std::cout << "Int" << std::endl;
	
	Point v = {p.x - startPosition.x, p.y - startPosition.y, p.z - startPosition.z};
	
	float dist = sqrt(pow(p.x - startPosition.x, 2) + pow(p.y - startPosition.y, 2) + pow(p.z - startPosition.z, 2));
	
	float res = 200;
	float pause = time / res;
	float per = 0;
	float inc = 1 / res;
	
	std::cout << "From: " << std::endl;
	startPosition.Print();
	std::cout << "To: " << std::endl;
	p.Print();
	std::cout << "Pauses: " << pause << std::endl;
	double start = time_time();
	for (int i = 0; i < res; i++){
		per += inc;
	
		Point n = { startPosition.x + per * v.x, startPosition.y + per * v.y, startPosition.z + per * v.z };
		n.Print();
		this->SetPosition(n, pause);
	}
	
	std::cout << "Elapsed time: " << time_time() - start << std::endl;
}

