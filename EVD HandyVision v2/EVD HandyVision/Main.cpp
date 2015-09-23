#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "OpenCVExtentions.h"
#include "HandReconisionTools.h"

#define PI 3.14159265359

int main(int argc, char** argb) {
	cv::Mat srcBGR, srcYCrCb, srcBinair, palmMask, fingers;

	//open image
	srcBGR = cv::imread("img1.jpg");
	if (!srcBGR.data)
		return -1;

	//YCbCr skin color filter
	cv::cvtColor(srcBGR, srcYCrCb, CV_RGB2YCrCb);
	YCbCrSkinColorFilter(srcYCrCb, srcBinair);

	cv::fill(srcBinair, srcBinair);

	cv::Point palmCenter;
	float palmRadius;
	getPalmCenter(srcBinair, palmCenter, palmRadius);
	cv::Point wristLeft, wristRight;
	findWrist(srcBinair, wristLeft, wristRight, palmCenter, palmRadius);
	getPalmMask(srcBinair, palmMask, palmCenter, palmRadius);

	cv::Point wristCenter = (wristLeft + wristRight) / 2;
	cv::Point handOrientation = palmCenter - wristCenter;
	handOrientation /= std::pow(handOrientation.x*handOrientation.x + handOrientation.y*handOrientation.y, 0.5f);
	getFingerMask(srcBinair, fingers, palmMask, wristCenter, handOrientation);

	//TODO: filter out small fingers
	//TODO: generate a bounding box for each finger
	//TODO: find if the thumb is visible, by looking for a finger that has s angle to the palmcenter that is bigger as 40 degree
	//TODO: find palmline(line that is parrelel to wristline and is just below all finger, except the thumb)
	//TODO: find wich finger is stretched
	//TODO: find the seperation space between fingers
	//TODO: determen definition of the sign 
	//TODO: display the image with the translation of the sign

	imshow("original", srcBGR);
	imshow("fingers", fingers);

	//Wait until a key is pressed to kill the program
	cv::waitKey(0);

	return 0;
}
