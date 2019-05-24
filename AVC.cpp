#include "E101.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int quadrant = 1;
const int leftMotor = 5;
const int rightMotor = 1;

void openGate(){
	char gateIP[] = "130.195.6.196";
	int gatePort = 1024;
	char request[] = "Please";
	char gatePassword[24];

	connect_to_server(gateIP, gatePort);
	send_to_server(request);
	receive_from_server(gatePassword);
	send_to_server(gatePassword);
}

/*
	Motor Speeds
	48 = stop
	30 = max anti-clockwise
	65 = max clockwise
*/

//Set motors to make robot go straight ahead
void forward() {
	set_motors(leftMotor, 33); //Left Motor
	set_motors(rightMotor, 63); //Right Motor
	hardware_exchange();
}

//Set motors to make robot go back
void reverse() {
	set_motors(leftMotor, 63);
	set_motors(rightMotor, 33);
	hardware_exchange();
}

//Set motors to make robot go left
void left(int blackTotal) {
	int speed;
	if (blackTotal > 5000) speed = 10;
	else if (blackTotal > 2000) speed = 7.5;
	else speed = 5;

	set_motors(leftMotor, 48-speed);
	set_motors(rightMotor, 48+speed*1.5);
	hardware_exchange();
}

//Set motors to make robot go right
void right(int blackTotal) {
	int speed;
	if (blackTotal > 5000) speed = 10;
	else if (blackTotal > 2000) speed = 7.5;
	else speed = 5;

	set_motors(leftMotor, 48-speed*1.5);
	set_motors(rightMotor, 48+speed);
	hardware_exchange();
}

// Make a 90 degree hard left turn
void hardLeft() {
	set_motors(leftMotor, 35); //Left Motor
	set_motors(rightMotor, 35); //Right Motor
	hardware_exchange();
}

// Make a 90 degree hard right turn
void hardRight() {
	set_motors(leftMotor, 61);
	set_motors(rightMotor, 61);
	hardware_exchange();
}

//Follow a black line
void followLine(){
	//const char* turns[] = {"r", "r", "l", "l", "r", "l", "r"};
	int camWidth = 320;
	int camHeight = 240;
	int leftBlack = 0;
	int middleBlack = 0;
	int rightBlack = 0;
	double sideSector = 1/3; // Size of left and right sections that camera is split into
	//const int LINE_WIDTH = 100; // Unsure of real line width
	int lineWidth = 0;

	take_picture();

	for (int y=0; y<camHeight; y++) {
			lineWidth = 0;
			for (int x=0; x<camWidth; x++) {

					if ((int)get_pixel(y, x, 3) < 100) {
							if (x < camWidth*sideSector) { //Test if pixel on left
									leftBlack++;
							} else if (x > camWidth*sideSector && x < camWidth*(1-sideSector)){ //Test is pixel in middle
									middleBlack++;
							} else if (x > camWidth*(1-sideSector)) { //Test if pixel on right
									rightBlack++;
							}
							lineWidth++;
					}
					
			}
	}

	int blackThreshold = 500;
	if (leftBlack > blackThreshold && rightBlack > blackThreshold && quadrant == 3) { //T-Intersection
			//Quad 3 stuff
	} else if (leftBlack > blackThreshold) { //If enough black on left, turn left
			left(leftBlack);
	} else if (rightBlack > blackThreshold) { //If enough black on right, turn right
			right(rightBlack);
	} else if(middleBlack < blackThreshold) { //If little to no black in middle sector, reverse back to re-find line
			reverse();
	}  else { //Go forward otherwise
			forward();
	}
}

//Get everything going
int main() {
	init(0);
	//open_screen_stream();
	while (true) {
		
			/*
			if (quadrant == 1) {
					openGate();
					quadrant = 2;
			} else if (quadrant == 2 || quadrant == 3) {
					followLine();
			} else if (quadrant == 4) {

			}
			*/
			
			followLine();
			
			/* Test motors
			set_motors(1,56);
			set_motors(5,40);
			hardware_exchange();
			sleep(5);
			set_motors(1,48);
			set_motors(5,48);
			hardware_exchange();*/
	}
	//close_screen_stream();
}
