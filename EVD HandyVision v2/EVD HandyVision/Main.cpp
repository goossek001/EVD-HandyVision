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

int DetermenGesture(std::string windowName, cv::Mat& srcBGR);
int main_photo(int argc, char** argb);
int main_video(int argc, char** argb);

int main(int argc, char** argb) {
	main_video(argc, argb);
}

int main_photo(int argc, char** argb) {
	cv::Mat srcBGR;
	// open image
	srcBGR = cv::imread("cam1.jpg");
	if (!srcBGR.data)
		return -1;

	int asdf = DetermenGesture("MyVideo", srcBGR);

	//Wait until a key is pressed to kill the program
	cv::waitKey(0);

	return asdf;
}

int main_video(int argc, char** argb) {
	cv::VideoCapture cap(0); // open the video camera no. 0

	if (!cap.isOpened())  // if not success, exit program
	{
		MessageBox(0, "Cannot open the video cam", 0, 0);
		return -1;
	}

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	cv::namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	if (!cap.isOpened())  // if not success, exit program
	{
		MessageBox(0, "Cannot open the video cam", 0, 0);
		return -1;
	}

	while (1) {
		cv::Mat frame;

		bool bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess) { //if not success, break loop
			MessageBox(0, "Cannot read a frame from video stream", 0, 0);
			break;
		}
		//cv::imshow("MyVideo", frame);

		DetermenGesture("MyVideo", frame);

		if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			MessageBox(0, "esc key is pressed by user", 0, 0);
			break;
		}
	}
	return 0;
}

int DetermenGesture(std::string windowName, cv::Mat& srcBGR) {
	cv::Mat srcYUV, srcBinair, palmMask, fingerMask;

	initHashTable();

	cv::cvtColor(srcBGR, srcYUV, CV_RGB2YCrCb);
	// Skin color filter
	YCbCrSkinColorFilter(srcYUV, srcBinair);

	// find palm
	cv::Point palmCenter;
	float palmRadius;
	getPalmCenter(srcBinair, palmCenter, palmRadius);
	createPalmMask(srcBinair, palmMask, palmCenter, palmRadius);

	// find wrist
	cv::Line wristLine;
	bool foundWrist = false;
	findWrist(srcBinair, wristLine, foundWrist, palmCenter, palmRadius);
	if (!foundWrist)
		return 1;

	cv::Point wristCenter = wristLine.position + wristLine.direction / 2;

	if (palmCenter == wristCenter)
		return 1;

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
	for (int i = 0; i < 5; ++i) {
		fingers[i] = 0;
	}
	int thumbIndex = getFindThumb(boundingBoxesFingers, palmCenter, handAngle, thumbDirection);
	if (thumbIndex >= 0)
		fingers[0] = &boundingBoxesFingers[thumbIndex];

	// find the palm line
	cv::Line palmLine;
	bool foundPalm = true;
	findPalmLine(srcBinair, palmLine, foundPalm, wristLine, palmRadius, handOrientation, fingers[0] != 0);
	if (!foundPalm)
		return 1;
	if (thumbDirection == Right) {
		palmLine.position = palmLine.lineEnd();
		palmLine.direction *= -1;
	}

	// find the 4 other fingers
	labelFingers(boundingBoxesFingers, fingers, wristCenter, handOrientation, palmLine);

	bool fingersStretch[5];
	areFingersStretched(fingers, fingersStretch, palmRadius);

	std::string gesture = deteremenGesture(GestureType::DutchCounting, fingersStretch);
	cv::putText(srcBGR, gesture, cv::Point(0.05f*srcBGR.cols, 0.95f*srcBGR.rows), 2, 0.01f*srcBGR.rows, cv::Scalar(255, 0, 0), 8);

	imshow(windowName, srcBGR);

	return 0;
}
