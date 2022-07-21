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


int main(){
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
	
	arm.SetAngle(&arm.joints[1], 180, 2);
	arm.SetAngle(&arm.joints[2], 225, 2);
	//arm.SetAngle(&arm.joints[1], 180, 2);
	//arm.SetAngle(&arm.joints[2], 225, 2);
	//arm.SetAngle(&arm.joints[3], 225, 2);

	arm.JoinThreads();
	
	arm.SetAngle(&arm.joints[1], 135, 4);
	arm.SetAngle(&arm.joints[2], 135, 4);
	//arm.SetAngle(&arm.joints[1], 135, 2);
	//arm.SetAngle(&arm.joints[2], 135, 2);
	//arm.SetAngle(&arm.joints[3], 135, 2);
	
	arm.JoinThreads();
	
	cout << "DONE!" << endl;
	
	gpioTerminate();
	return 0;
}
