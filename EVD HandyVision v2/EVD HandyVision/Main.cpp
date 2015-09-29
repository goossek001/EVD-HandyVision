#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "OpenCVExtentions.h"
#include "HandReconisionTools.h"
#include "Math.h"

int main(int argc, char** argb) {
	cv::Mat srcBGR, srcYCrCb, srcBinair, palmMask, fingerMask;

	//open image
	srcBGR = cv::imread("img3.jpg");
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
	cv::Point2f handOrientation = (cv::Point2f)palmCenter - (cv::Point2f)wristCenter;
	handOrientation /= std::pow(handOrientation.x*handOrientation.x + handOrientation.y*handOrientation.y, 0.5f);
	float handAngle = std::atan2(handOrientation.y, handOrientation.x);
	getFingerMask(srcBinair, fingerMask, palmMask, wristCenter, handOrientation);

	//TODO: filter out small fingers

	std::vector<cv::RotatedRect> boundingBoxesFingers = getBoundingBoxes(fingerMask);
	ThumbDirection thumbDirection = Right;
	int thumbIndex = getFindThumb(boundingBoxesFingers, palmCenter, handAngle, thumbDirection);

	cv::Point palmLineStart, palmLineEnd;
	getPalmLine(srcBinair, palmLineStart, palmLineEnd, wristLeft, wristRight, handOrientation, thumbIndex >= 0);
	if (thumbDirection == Right) {
		cv::Point temp = palmLineStart;
		palmLineStart = palmLineEnd;
		palmLineEnd = temp;
	}


	int indexFingerIndex = -1;
	int middleFingerIndex = -1;
	int ringFingerIndex = -1;
	int pinkyIndex = -1;
	float palmWidth = math::length(palmLineEnd - palmLineStart);
	for (int i = 0; i < boundingBoxesFingers.size(); ++i) {
		if (i != thumbIndex) {
			float cloasestDistance = 9999;
			cv::Point fingerPosition;
			cv::Point2f vertices[4];
			boundingBoxesFingers[i].points(vertices);
			for (int j = 0; j <= 4; ++j) {
				cv::Point point = (cv::Point)(vertices[j] + vertices[(j + 1) % 4]) / 2;
				float distance = math::length(point - wristCenter);
				if (distance < cloasestDistance) {
					fingerPosition = point;
					cloasestDistance = distance;
				}
			}

			cv::Point intersect = math::lineLineIntersection(cv::Line(fingerPosition, -handOrientation), cv::Line(palmLineStart, palmLineEnd - palmLineStart));
			int finger = 1 + math::length(intersect - palmLineStart) / palmWidth * 4;
			if (finger <= 1)
				indexFingerIndex = i;
			else if (finger == 2)
				middleFingerIndex = i;
			else if (finger == 3)
				ringFingerIndex = i;
			else 
				pinkyIndex = i;
		}
	}


	//TODO: generate a bounding box for each finger
	//TODO: find if the thumb is visible, by looking for a finger that has s angle to the palmcenter that is bigger as 40 degree
	//TODO: find palmline(line that is parrelel to wristline and is just below all finger, except the thumb)
	//TODO: find wich finger is stretched
	//TODO: find the seperation space between fingers
	//TODO: determen definition of the sign 
	//TODO: display the image with the translation of the sign

	imshow("original", srcBGR);
	imshow("binair", srcBinair);
	imshow("fingers", fingerMask);

	Mat thumb, indexFinger, middleFinger, ringFinger, pinky;
	cv::applyRectangleMask(fingerMask, thumb, boundingBoxesFingers[thumbIndex]);
	imshow("thumb", thumb);
	cv::applyRectangleMask(fingerMask, indexFinger, boundingBoxesFingers[indexFingerIndex]);
	imshow("indexFinger", indexFinger);
	cv::applyRectangleMask(fingerMask, middleFinger, boundingBoxesFingers[middleFingerIndex]);
	imshow("middleFinger", middleFinger);
	cv::applyRectangleMask(fingerMask, ringFinger, boundingBoxesFingers[ringFingerIndex]);
	imshow("ringFinger", ringFinger);
	cv::applyRectangleMask(fingerMask, pinky, boundingBoxesFingers[pinkyIndex]);
	imshow("pinky", pinky);

	//Wait until a key is pressed to kill the program
	cv::waitKey(0);

	return 0;
}
