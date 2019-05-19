#include "E101.h"
#include <iostream>

using namespace std;

int quadrant = 1;

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
	set_motor(2, 33); //Left Motor
	set_motor(1, 63); //Right Motor
}

//Set motors to make robot go back
void reverse() {
	set_motor(2, 63); //Left Motor
	set_motor(1, 33); //Right Motor
}

//Set motors to make robot go left
void left(int blackTotal) {
	int speed;
	if (blackTotal > 5000) speed = 10;
	else if (blackTotal > 2000) speed = 7.5;
	else speed = 5;
	
	set_motor(2, 48-speed); //Left Motor
	set_motor(1, 48+speed*1.5); //Right Motor
}

//Set motors to make robot go right
void right(int blackTotal) {
	int speed;
	if (blackTotal > 5000) speed = 10;
	else if (blackTotal > 2000) speed = 7.5;
	else speed = 5;
	
	set_motor(2, 48-speed*1.5); //Left Motor
	set_motor(1, 48+speed); //Right Motor
}

void followLine(){
	//const char* turns[] = {"r", "r", "l", "l", "r", "l", "r"};
	int width = 320;
	int height = 240;
	int leftBlack = 0;
	int middleBlack = 0;
	int rightBlack = 0;

	take_picture();
	
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			
			if ((int)get_pixel(y, x, 3) < 100) {
				if (x < width*(1/3)) { //Test if pixel on left
					leftBlack++;
				} else if (x > width*(1/3) && x < width*(2/3)){ //Test is pixel in middle
					middleBlack++;
				} else if (x > width*(2/3)) { //Test if pixel on right
					rightBlack++;
				}
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
	init();
	
	while (true) {
		if (quadrant == 1) {
			openGate();
			quadrant ++;
		} else if (quadrant == 2 || quadrant == 3) {
			followLine();
		} else if (quadrant == 4) {
			
		}
	}
}
