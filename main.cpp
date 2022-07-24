#include <iostream>	
#include <stdlib.h>
#include <thread>
#include <pigpio.h>
#include <vector>
#include "Joint.h"
#include "Arm.h"

#define MOTOR_A 12	
#define MOTOR_B 18
#define MOTOR_C 19
#define MOTOR_D 13

using namespace std;

// MAX SPEED TIME = 0.0003
// MIN SPEED TIME = 0.005

// 500 = 0
// 1500 = 180
// 2500 = 270

int motorPositionA = 1500;
int motorPositionB = 1500;
int motorPositionC = 1500;
int motorPositionD = 1500;

vector<thread> waitingThreads; 


int main(int argc, char** argv){
	if (gpioInitialise() < 0){
		cout << "Failed to initialise!" << endl;
	}
	else {
		cout << "Successfully initialised GPIO!" << endl;
	}
	
	
	gpioServo(MOTOR_A, motorPositionA);
	gpioServo(MOTOR_B, motorPositionB);
	gpioServo(MOTOR_C, motorPositionC);
	gpioServo(MOTOR_D, motorPositionD);
	time_sleep(1);
	
	Arm arm;
	arm.AddJoint(Joint(MOTOR_A));
	arm.AddJoint(Joint(MOTOR_B));
	arm.AddJoint(Joint(MOTOR_C));
	arm.AddJoint(Joint(MOTOR_D));
	
	cout << argc << endl;
	
	Arm::Point p1 = {15, 0, 10};
	Arm::Point p2 = {25, 0, 10};
	Arm::Point p3 = {25, 0, -10};
	Arm::Point p4 = {15, 0, -10};
	Arm::Point p5 = {15, 0, 10};
	//Arm::Point p2 = {(float)atof(argv[1]), (float)atof(argv[2]), (float)atof(argv[3])};
	
	
	arm.SetPosition(p1, 2);
	arm.JoinThreads();
	time_sleep(1);
	
	arm.Interpolate(p2, 2);
	time_sleep(1);
	arm.Interpolate(p3, 2);
	time_sleep(1);
	arm.Interpolate(p4, 2);
	time_sleep(1);
	arm.Interpolate(p5, 2);
	time_sleep(1);
	
	arm.SetAngle(&arm.joints[0], 135, 2);
	arm.SetAngle(&arm.joints[1], 135, 2);
	arm.SetAngle(&arm.joints[2], 135, 2);
	
	arm.JoinThreads();
	
	cout << "DONE!" << endl;
	
	gpioTerminate();
	return 0;
}
