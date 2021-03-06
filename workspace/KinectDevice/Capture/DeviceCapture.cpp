/*
 * CameraCapture.cpp
 *
 *  Created on: 30/12/2013
 *      Author: rgap
 */

#include "DeviceCapture.h"
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace cv;

#define NORMALSIZE cv::Size(640,480)

//////////////////////////////////////

DeviceCapture::DeviceCapture(int CAPTURE_TYPE, const char*path_img_bgr, const char*path_img_depth) {

	CAPTURE_TYPE_ = CAPTURE_TYPE;

	if (CAPTURE_TYPE < 2) {
		if (CAPTURE_TYPE)
			capture.open(CV_CAP_OPENNI);
		else
			capture.open(0);

		if (!capture.isOpened()) {
			std::cout << "Can not open a capture object." << std::endl;
			std::exit(1);
		}

		szFrame = cv::Size((int) capture.get(CV_CAP_PROP_FRAME_WIDTH),
				(int) capture.get(CV_CAP_PROP_FRAME_HEIGHT));

		imgBGR = cv::Mat(szFrame, CV_8UC3);
		world = cv::Mat(szFrame, CV_32FC3);

	} else {
		imgBGR = cv::imread(path_img_bgr);
		if (imgBGR.empty()) {
			std::cout << "Can not read : " << path_img_bgr << std::endl;
			std::exit(1);
		}
		szFrame = imgBGR.size();

		Mat depth = cv::imread(path_img_depth);
		world = cv::Mat(szFrame, CV_32FC3);

		int i,j;
		for (i = 0; i < depth.rows; i++) {
			float*depth_pt = depth.ptr<float> (i);
			Vec3f *world_ptr = world.ptr<Vec3f>(i);
			for (j = 0; j < depth.cols; j++) {
				world_ptr[j][0] = i;
				world_ptr[j][1] = j;
				world_ptr[j][2] = depth_pt[j];
			}
		}

		if (world.empty()) {
			std::cout << "Can not read : " << path_img_depth << std::endl;
			std::exit(1);
		}
	}
}

void DeviceCapture::verifyState() {
	if (!capture.grab() && CAPTURE_TYPE_ < 2) {
		std::cout << "Can not grab images." << std::endl;
		std::exit(1);
	}
}

cv::Mat DeviceCapture::getBGRFrame() {
	if (CAPTURE_TYPE_ < 2) {
		if (CAPTURE_TYPE_)
			capture.retrieve(imgBGR, CV_CAP_OPENNI_BGR_IMAGE);
		else
			capture >> imgBGR;
		return imgBGR;
	}else
		return imgBGR.clone();

}

cv::Mat DeviceCapture::getDepthFrame() {
	if(CAPTURE_TYPE_ < 2){
		capture.retrieve(world, CV_CAP_OPENNI_POINT_CLOUD_MAP);
		return world;
	}
	else
		return world.clone();
}

void DeviceCapture::showCameraFeatures() {
	cout << "\nDepth generator output mode:" << endl << "FRAME_WIDTH      "
			<< capture.get(CV_CAP_PROP_FRAME_WIDTH) << endl
			<< "FRAME_HEIGHT     " << capture.get(CV_CAP_PROP_FRAME_HEIGHT)
			<< endl << "FRAME_MAX_DEPTH  " << capture.get(
			CV_CAP_PROP_OPENNI_FRAME_MAX_DEPTH) << " mm" << endl
			<< "FPS              " << capture.get(CV_CAP_PROP_FPS) << endl
			<< "REGISTRATION     " << capture.get(
			CV_CAP_PROP_OPENNI_REGISTRATION) << endl;
	if (capture.get(CV_CAP_OPENNI_IMAGE_GENERATOR_PRESENT)) {
		cout << "\nImage generator output mode:" << endl << "FRAME_WIDTH   "
				<< capture.get(
						CV_CAP_OPENNI_IMAGE_GENERATOR + CV_CAP_PROP_FRAME_WIDTH)
				<< endl << "FRAME_HEIGHT  " << capture.get(
				CV_CAP_OPENNI_IMAGE_GENERATOR + CV_CAP_PROP_FRAME_HEIGHT)
				<< endl << "FPS           " << capture.get(
				CV_CAP_OPENNI_IMAGE_GENERATOR + CV_CAP_PROP_FPS) << endl;
	}
}

DeviceCapture::~DeviceCapture() {
}
