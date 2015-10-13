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

int main(int argc, char** argb) {
	cv::Mat srcBGR, srcYUV, srcBinair, palmMask, fingerMask;

	// open image
	srcBGR = cv::imread("img13.jpg");
	if (!srcBGR.data)
		return -1;

	cv::cvtColor(srcBGR, srcYUV, CV_RGB2YCrCb);
	// Skin color filter
	YCbCrSkinColorFilter(srcYUV, srcBinair);

	// find biggest contour return binair.
	getContour(srcBinair, srcBinair);

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
	int thumbIndex, indexFingerIndex, middleFingerIndex, ringFingerIndex, pinkyIndex;
	thumbIndex = getFindThumb(boundingBoxesFingers, palmCenter, handAngle, thumbDirection);

	// find the palm line
	cv::Line palmLine;
	findPalmLine(srcBinair, palmLine, wristLine, palmRadius, handOrientation, thumbIndex >= 0);
	if (thumbDirection == Right) {
		palmLine.position = palmLine.lineEnd();
		palmLine.direction *= -1;
	}

	// find the 4 other fingers
	labelFingers(boundingBoxesFingers, wristCenter, handOrientation, palmLine, thumbIndex, indexFingerIndex
		, middleFingerIndex, ringFingerIndex, pinkyIndex);


	//TODO: find wich finger is stretched
	//TODO: determen definition of the sign 
	//TODO: display the image with the translation of the sign

	//Display the generated images
	cv::line(srcBinair, wristLine.lineStart(), wristLine.lineEnd(), cv::Scalar(100, 0, 0));
	imshow("original", srcBGR);
	imshow("binair", srcBinair);
	imshow("fingers", fingerMask);

	//Create a seperate image of each finger
	Mat thumb, indexFinger, middleFinger, ringFinger, pinky;
	if (thumbIndex >= 0) {
		cv::applyRectangleMask(fingerMask, thumb, boundingBoxesFingers[thumbIndex]);
		imshow("thumb", thumb);
	}
	if (indexFingerIndex >= 0){
		cv::applyRectangleMask(fingerMask, indexFinger, boundingBoxesFingers[indexFingerIndex]);
		imshow("indexFinger", indexFinger);
	}
	if (middleFingerIndex >= 0){
		cv::applyRectangleMask(fingerMask, middleFinger, boundingBoxesFingers[middleFingerIndex]);
		imshow("middleFinger", middleFinger);
	}
	if (ringFingerIndex >= 0){
		cv::applyRectangleMask(fingerMask, ringFinger, boundingBoxesFingers[ringFingerIndex]);
		imshow("ringFinger", ringFinger);
	}
	if (pinkyIndex >= 0){
		cv::applyRectangleMask(fingerMask, pinky, boundingBoxesFingers[pinkyIndex]);
		imshow("pinky", pinky);
	}

	//Wait until a key is pressed to kill the program
	cv::waitKey(0);

	return 0;
}
