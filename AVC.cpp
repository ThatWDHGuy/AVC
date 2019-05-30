#include "E101.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int quadrant = 1;
const int leftMotor = 5;
const int rightMotor = 1;
int speed = 0;

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
void setForward() {
        cout<<"Setting motor forward"<<endl;
        set_motors(leftMotor, 40);
        set_motors(rightMotor, 56);
        hardware_exchange();
}

//Set motors to make robot go back
void setReverse() {
        cout<<"Setting motor reverse"<<endl;
        set_motors(leftMotor, 56);
        set_motors(rightMotor, 40);
        hardware_exchange();
}

// Make a 90 degree hard left turn
void doHardLeft() {
        cout<<"Turning Hard Left"<<endl;
        set_motors(leftMotor, 35);
        set_motors(rightMotor, 35);
        hardware_exchange();
        sleep(1);
        set_motors(leftMotor, 48);
        set_motors(rightMotor, 48);
        hardware_exchange();
}

// Make a 90 degree hard right turn
void doHardRight() {
        cout<<"Turning Hard Right"<<endl;
        set_motors(leftMotor, 61);
        set_motors(rightMotor, 61);
        hardware_exchange();
        sleep(1);
        set_motors(leftMotor, 48);
        set_motors(rightMotor, 48);
        hardware_exchange();
}

void turnLeft() {
        cout<<"Turning Left"<<endl;
        set_motors(leftMotor, 48);
        set_motors(rightMotor, 32);
        hardware_exchange();
}

void turnRight() {
        cout<<"Turning Right"<<endl;
        set_motors(leftMotor, 60);
        set_motors(rightMotor, 48);
        hardware_exchange();
}

const int camWidth = 320;
const int camHeight = 240;
const int turns[] = {0, 0, 1, 1, 0, 1, 0}; // 0 = right, 1 = left
int turnsTaken = 0;
int nextQuad = 0;

void followLineQ2() { //Q2
        int linePos = 0;
        take_picture();
        //update_screen(); TESTING

        for (int x=0; x<camWidth; x++) {
                if ((int)get_pixel(((int)camHeight/2), x, 3) < 100) {
                        if (x < camWidth/2){
                                linePos--;
                        } else {
                                linePos++;
                        }
                }
                if ((int)get_pixel(((int)camHeight/2), x, 0) > 100 &&
		    (int)get_pixel(((int)camHeight/2), x, 1) < 100 &&
		    (int)get_pixel(((int)camHeight/2), x, 2) < 100){ //check if red pixel change 150 to valued discovered (do testing)
                        nextQuad++;
                }
        }
        cout<<linePos<<endl;
        if (linePos == 0){
                setForward();
        } else if (linePos > 0) {
                turnRight();
        } else if (linePos < 0) {
                turnLeft();
        }
}

void followLineQ3() { //Q3
	int stillLine = 0;
        int linePos = 0;
        take_picture();
        //update_screen(); TESTING

        for (int x=0; x<camWidth; x++) {
                if ((int)get_pixel(((int)camHeight/2), x, 3) < 100) {
                        if (x < camWidth/2){
                                linePos--;
                        } else {
                                linePos++;
                        }	
			stillLine++;
                }
		
        }
        cout<<linePos<<endl;
	cout<<stillLine<<endl;
	if (stillLine != 0){
		if (linePos == 0){
			setForward();
	        } else if (linePos > 0) {
	                turnRight();
	        } else if (linePos < 0) {
	                turnLeft();
	        }
	} else {
		if (turns[turnsTaken] == 0){//make do Hard turns inturrupt and do a turn for 0.3ish secs
			doHardTurnRight();
		} else {
			doHardTurnLeft(); //make do Hard turns inturrupt and do a turn for 0.3ish secs
		}
	}
}

//Get everything going
int main() {
        init(0);
        //open_screen_stream(); TESTING
        quadrant = 2;
        while (true) {
                        if (quadrant == 1) {
                                openGate();
                                quadrant++;
                                cout<<"Quad 2 Time"<<endl;
                        } else if (quadrant == 2){
                                followLineQ2();
                                if (nextQuad == 1){
                                        nextQuad = 0;
                                        quadrant++;
                                        cout<<"Quad 3 Time"<<endl;
                                }
                        } else if (quadrant == 3) {
				followLineQ3();
                        } else if (quadrant == 4) {

                        }

                        /* TESTING
                        set_motors(1,56);
                        set_motors(5,40);
                        hardware_exchange();
                        sleep(5);
                        set_motors(1,48);
                        set_motors(5,48);
                        hardware_exchange();
                        */
        }
        //close_screen_stream(); TESTING
        return 0;
}
