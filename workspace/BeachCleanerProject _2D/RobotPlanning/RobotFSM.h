/*
 * RobotFSM.h
 *
 *  Created on: 02/04/2014
 *      Author: rgap
 */

#ifndef ROBOTFSM_H_
#define ROBOTFSM_H_

#include <opencv2/opencv.hpp>
#include "ActionControl.h"
#include "../ModelParameters.h"
#include "../globals.h"
using namespace cv;
using namespace std;

class RobotFSM {
private:

	Rect *nearestObstacle;

	ActionControl control;
	ModelParameters* P;
public:
	RobotFSM();
	void initializeDevice(std::string devicePath);
	void setParameters(ModelParameters* P_);
	void collectCan(Rect *rectNearestCan);

	void moveStraight(Rect *rectNearestCan, int H);
	void searchCan(Rect *rectNearestCan, int canCenterX, int W, int H);

	void planWithEnvironment(Rect *rectNearestCan, Mat &imgColor);
	virtual ~RobotFSM();
};

#endif /* ROBOTFSM_H_ */