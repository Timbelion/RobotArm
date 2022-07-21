#include <iostream>	
#include <stdlib.h>
#include <pigpio.h>
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
