#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "OpenCVExtentions.h"
#include "HandReconisionTools.h"

#define PI 3.14159265359

int main(int argc, char** argb) {
	cv::Mat srcBGR, srcYCrCb, srcBinair;

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


	imshow("original", srcBGR);

	Mat wristLine(srcBinair);
	cv::line(wristLine, wristLeft, wristRight, cv::Scalar(100), 4);
	imshow("wrist", wristLine);

	//Wait until a key is pressed to kill the program
	cv::waitKey(0);

	return 0;
}
