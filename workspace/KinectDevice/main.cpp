#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "RawKinect/RawKinect.h"
#include "Capture/DeviceCapture.h"

using namespace cv;
using namespace std;

#define MOTOR_ABAJO_ANGULO -102
#define MOTOR_ARRIBA_ANGULO -70

void frameLoop() {
	DeviceCapture device(1);
	RawKinect rawKinect;
	rawKinect.Open();
	//////////////////////////

	float x, y, z;
	int k;
	while (1) {
		device.verifyState();
		imshow("imgBGR", device.getBGRFrame());

		k = waitKey(1); // 0.1 sec --- milisec
		if (k == 27) {
			break;
		} else if (k == 'w') {
			rawKinect.MoveInc(-2);
		} else if (k == 'e') {
			rawKinect.MoveInc(2);
		} else if (k == 's') {
			rawKinect.Move(MOTOR_ARRIBA_ANGULO, 1); // + 40 grads separacion
			cout << "MOTOR ARRIBA " << endl;
		} else if (k == 'd') {
			rawKinect.Move(MOTOR_ABAJO_ANGULO, 1);
			cout << "MOTOR ABAJO " << endl;
		} else if (k == 'a') {
			rawKinect.getAccelerometer(x, y, z);
			cout << "x = " << x << ", y = " << y << ", z = " << z << endl;
		}
	}
}

int main() {
	frameLoop();
}
