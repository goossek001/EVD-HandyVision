//***************************************************************************************
// Load a image and find all visible fingers in this image
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.00
//***************************************************************************************

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "OpenCVExtentions.h"
#include "HandReconisionTools.h"
#include "Math.h"

#include <Windows.h>

int main(int argc, char** argb) {
	cv::Mat srcBGR, srcYUV, srcBinair, palmMask, fingerMask;

	// open image
	srcBGR = cv::imread("img2.jpg");
	if (!srcBGR.data)
		return -1;

	initHashTable();

	cv::cvtColor(srcBGR, srcYUV, CV_RGB2YCrCb);
	// Skin color filter
	YCbCrSkinColorFilter(srcYUV, srcBinair);

	//TODO: Reconise hand blob and remove all others
	//TODO: Change ROI

	// find palm
	cv::Point palmCenter;
	float palmRadius;
	getPalmCenter(srcBinair, palmCenter, palmRadius);
	createPalmMask(srcBinair, palmMask, palmCenter, palmRadius);

	// find wrist
	cv::Line wristLine;
	findWrist(srcBinair, wristLine, palmCenter, palmRadius);
	cv::Point wristCenter = wristLine.position + wristLine.direction / 2;

	// find the orientation of the hand
	cv::Point2f handOrientation = (cv::Point2f)palmCenter - (cv::Point2f)wristCenter;
	handOrientation /= std::pow(handOrientation.x*handOrientation.x + handOrientation.y*handOrientation.y, 0.5f);
	float handAngle = std::atan2(handOrientation.y, handOrientation.x);

	// find the fingers
	createFingerMask(srcBinair, fingerMask, palmMask, wristCenter, handOrientation);
	std::vector<cv::RotatedRect> boundingBoxesFingers = getBoundingBoxes(fingerMask);

	//TODO: determen the direction of the thumb
	ThumbDirection thumbDirection = Right;

	// find the thumb
	cv::RotatedRect* fingers[5];
	std::fill(fingers, fingers + 5, nullptr);
	int thumbIndex = getFindThumb(boundingBoxesFingers, palmCenter, handAngle, thumbDirection);
	if (thumbIndex >= 0)
		fingers[0] = &boundingBoxesFingers[thumbIndex];

	// find the palm line
	cv::Line palmLine;
	findPalmLine(srcBinair, palmLine, wristLine, palmRadius, handOrientation, fingers[0]);
	if (thumbDirection == Right) {
		palmLine.position = palmLine.lineEnd();
		palmLine.direction *= -1;
	}

	// find the 4 other fingers
	labelFingers(boundingBoxesFingers, fingers, wristCenter, handOrientation, palmLine);


	//TODO: find wich finger is stretched
	//TODO: determen definition of the sign 
	//TODO: display the image with the translation of the sign

	//Display the generated images
	imshow("original", srcBGR);
	imshow("binair", srcBinair);
	imshow("fingers", fingerMask);

	displayFingers(srcBGR, fingers);

	bool fingersStretch[5];
	areFingersStretched(fingers, fingersStretch, palmRadius);

	std::string gesture = deteremenGesture(GestureType::DutchCounting, fingersStretch);
	MessageBox(0, gesture.c_str(), 0, 0);

	//Wait until a key is pressed to kill the program
	cv::waitKey(0);

	return 0;
}
