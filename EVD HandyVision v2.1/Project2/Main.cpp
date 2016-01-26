
#include "HandReconisionTools.h"
#include "Math.h"

int DetermenGesture(std::string windowName, cv::Mat& cvSrcBGR);
int main_video();
int main_photo();

int main() {
	return main_photo();
}

int main_photo() {
	cv::Mat srcBGR;
	// open image
	srcBGR = cv::imread("img8.jpg");
	if (!srcBGR.data)
		return -1;

	int r = DetermenGesture("MyVideo", srcBGR);

	//Wait until a key is pressed to kill the program
	cv::waitKey(0);

	return r;
}

int main_video() {
	cv::VideoCapture cap(0); // open the video camera no. 0

	if (!cap.isOpened()) 
		throw "Cannot open the video cam";

	cv::namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	while (1) {
		cv::Mat frame;

		bool bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess)  //if not success, break loop
			throw "Cannot read a frame from video stream";

		DetermenGesture("MyVideo", frame);

		if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			break;
	}
	return 0;
}

int DetermenGesture(std::string windowName, cv::Mat& cvSrcBGR) {
	vision::Mat srcHSV, srcBinair, palmMask, fingerMask;
	vision::Mat srcBGR = vision::Mat(cvSrcBGR);
	initHashTable();

	vision::morphologyEx(srcBGR, srcBGR, vision::GAUSSIAN, 7);
	vision::bgrtohsv(srcBGR, srcHSV);
	// Skin color filter
	int H_min = 236, H_max = 40, S_min = 33, S_max = 241, V_min = 30, V_max = 222, S_size = 128, V_size = 128;
	adaptiveHSVSkinColorFilter(srcHSV, srcBinair, H_min, H_max, S_min, S_max, V_min, V_max, S_size, V_size);
	
	vision::morphologyEx(srcBinair, srcBinair, vision::CLOSE, 5);
	
	vision::fillHoles(srcBinair, srcBinair, vision::FOUR);
	vision::Mat temp = vision::Mat(srcBinair);
	vision::setSelectedValue(temp, temp, 1, 255);
	cv::Mat cvFingerMask = temp;
	cv::imshow("asdffa", cvFingerMask);
	cv::waitKey(0);

	// find palm
	vision::Point palmCenter;
	float palmRadius;
	getPalmCenter(srcBinair, palmCenter, palmRadius);

	createPalmMask(srcBinair, palmMask, palmCenter, palmRadius);

	// find wrist
	vision::Line wristLine;
	bool foundWrist = false;
	findWrist(srcBinair, wristLine, foundWrist, palmCenter, palmRadius);

	if (!foundWrist)
		return 1;


	vision::Point wristCenter = wristLine.position + wristLine.direction / 2;

	if (palmCenter == wristCenter)
		return 1;

	// find the orientation of the hand
	vision::Point2f handOrientation = (vision::Point2f)palmCenter - (vision::Point2f)wristCenter;
	handOrientation = handOrientation / std::pow(handOrientation.x*handOrientation.x + handOrientation.y*handOrientation.y, 0.5f);
	float handAngle = std::atan2(handOrientation.y, handOrientation.x);

	// find the fingers
	createFingerMask(srcBinair, fingerMask, palmMask, wristCenter, handOrientation);
	std::vector<cv::RotatedRect> boundingBoxesFingers = vision::getBoundingBoxes(fingerMask);

	//TODO: determen the direction of the thumb
	ThumbDirection thumbDirection = ThumbDirection::Right;

	// find the thumb
	cv::RotatedRect* fingers[5];
	std::fill(fingers, fingers + 5, (cv::RotatedRect*)0);

	int thumbIndex = findThumb(boundingBoxesFingers, palmCenter, handAngle, thumbDirection);
	if (thumbIndex >= 0)
		fingers[0] = &boundingBoxesFingers[thumbIndex];

	// find the palm line
	vision::Line palmLine;
	bool foundPalm = true;
	findPalmLine(srcBinair, palmLine, foundPalm, wristLine, palmRadius, handOrientation, thumbIndex >= 0);
	if (!foundPalm)
		return 1;
	if (thumbDirection == ThumbDirection::Right) {
		palmLine.position = palmLine.lineEnd();
		palmLine.direction = palmLine.direction * - 1;
	}
	
	// find the 4 other fingers
	labelFingers(boundingBoxesFingers, fingers, wristCenter, handOrientation, palmLine);

	bool fingersStretch[5];
	areFingersStretched(fingers, fingersStretch, palmRadius);

	std::string gesture = deteremenGesture(GestureType::DutchCounting, fingersStretch);
	
	Mat finalImage = srcBGR;
	cv::Line cvWristLine(cv::Point(wristLine.position.x, wristLine.position.y), cv::Point(wristLine.direction.x, wristLine.direction.y));
	cv::Line cvPalmLine(cv::Point(palmLine.position.x, palmLine.position.y), cv::Point(palmLine.direction.x, palmLine.direction.y));
	cv::line(finalImage, cvPalmLine.lineStart(), cvPalmLine.lineEnd(), cv::Scalar(150));
	cv::line(finalImage, cvWristLine.lineStart(), cvWristLine.lineEnd(), cv::Scalar(0, 150));
	if (gesture.size() > 0)
		cv::putText(finalImage, gesture, cv::Point(0.05f*finalImage.cols, 0.95f*finalImage.rows), 2, 0.006f*finalImage.rows, cv::Scalar(255, 255, 255), 8);
	imshow(windowName, finalImage);

	return 0;
}
