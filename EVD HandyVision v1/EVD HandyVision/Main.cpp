#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "OpenCVExtentions.h"
#include "HandReconisionTools.h"

#define PI 3.14159265359

int main(int argc, char** argb) {
	cv::Mat srcBGR, srcYCrCb, srcGray, srcBinair, srcOuterEdge, srcCannyEdge, srcFullEdge, srcFingers;
	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	char* window_name = "Laplace Demo";

	//open image
	srcBGR = cv::imread("img1.jpg");
	if (!srcBGR.data)
		return -1;

	//YCbCr skin color filter
	cv::cvtColor(srcBGR, srcYCrCb, CV_RGB2YCrCb);
	YCbCrSkinColorFilter(&srcYCrCb, &srcBinair);

	//TODO: Apply a HSV skin color filter and do a AND operator with it and the YCbCr skin color filter

	//TODO: remove big skin areas that arent hands
	//TODO: clip the img

	//Detect outer contour
	detectAndDrawContour(&srcBinair, &srcOuterEdge, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//Apply canny filter
	cv::cvtColor(srcBGR, srcGray, CV_RGB2GRAY);
	cv::Canny(srcGray, srcCannyEdge, 120, 255);

	//Combine the outercontour with the canny edge
	cv::bitwise_or(srcOuterEdge, srcCannyEdge, srcFullEdge);

	//TODO: estimate finger size
	float fingerThickness = 8;

	//Find fingers
	findFingers(&srcFullEdge, &srcFingers, &srcBinair, fingerThickness);

	//Find the center of mass of the fingers
	cv::Point fingerCenterOfMass = cv::getCenterOfMass(&srcFingers);

	//Clip
	cv::Point topLeft = cv::Point(std::max(0.0f, fingerCenterOfMass.x - fingerThickness * 10), std::max(0.0f, fingerCenterOfMass.y - fingerThickness * 10));
	cv::Point bottomRight = cv::Point(std::min((float)srcBinair.cols, fingerCenterOfMass.x + fingerThickness * 10), std::min((float)srcBinair.rows, fingerCenterOfMass.y + fingerThickness * 10));
	cv::Rect ROI(topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);
	fingerCenterOfMass -= topLeft;

	Mat srcCroppedBinair = srcBinair(ROI);
	Mat srcCroppedFingers = srcFingers(ROI);
	Mat srcCroppedEdge = srcOuterEdge(ROI);
	Mat srcCroppedBGR = srcBGR(ROI);

	cv::Mat srcBinairWithoutArm;

	//Remove the arm
	removeArm(&srcCroppedBinair, &srcBinairWithoutArm, fingerCenterOfMass, fingerThickness);

	//Find hand center and its orientation
	cv::Point2f handCenter = getHandCenter(&srcBinairWithoutArm);
	cv::Point2f handOrientatation(fingerCenterOfMass.x - handCenter.x, fingerCenterOfMass.y - handCenter.y);
	handOrientatation /= sqrt(handOrientatation.x*handOrientatation.x + handOrientatation.y*handOrientatation.y);

	//Find wrist position
	cv::Point wristPosition = fingerCenterOfMass - (cv::Point)(10 * fingerThickness * handOrientatation);

	//Count pixels in all directions from the wrist, to find the fingers
	float handRotation = std::atan2f(handOrientatation.y, handOrientatation.x);
	const int numberOfRays = 362;
	int pixelCounts[numberOfRays] = { 0 };
	const float deltaAngle = 181.0f / numberOfRays;
	for (float i = -90; i <= 90; i += deltaAngle) {
		float rotation = handRotation + i / 180.0f * PI;
		cv::Point2f direction(std::cos(rotation), std::sin(rotation));
		float dx;
		float dy;
		if (std::abs(direction.x) >= std::abs(direction.y)) {
			dx = direction.x > 0? 1: -1;
			dy = direction.y / std::abs(direction.x);
		} else {
			dx = direction.x / std::abs(direction.y);
			dy = direction.y > 0 ? 1 : -1;
		}

		float x = wristPosition.x;
		float y = wristPosition.y;

		while (x >= 0 && x < srcCroppedFingers.cols && y >= 0 && y < srcCroppedFingers.rows) {
			pixelCounts[(int)((i + 90)/deltaAngle)] += (srcCroppedFingers.at<uchar>(cv::Point(x, y)) > 0 ? 1 : 0);

			x += dx;
			y += dy;
		}
	}

	std::vector<int> peaks = cv::getHighestPeaks(pixelCounts, numberOfRays, 5);

	std::vector<cv::Point> fingerTips(peaks.size());
	for (int i = 0; i < fingerTips.size(); ++i) {
		float rotation = handRotation + (peaks[i] * deltaAngle - 90) / 180.0f * PI;
		cv::Point2f direction = cv::Point2f(std::cos(rotation), std::sin(rotation));
		float dx;
		float dy;
		if (std::abs(direction.x) >= std::abs(direction.y)) {
			dx = direction.x > 0 ? 1 : -1;
			dy = direction.y / std::abs(direction.x);
		} else {
			dx = direction.x / std::abs(direction.y);
			dy = direction.y > 0 ? 1 : -1;
		}

		float x = wristPosition.x;
		float y = wristPosition.y;
		
		while (x >= 0 && x < srcCroppedFingers.cols && y >= 0 && y < srcCroppedFingers.rows) {
			if (srcCroppedEdge.at<uchar>(cv::Point(x, y))
					|| x - 1 >= 0 && srcCroppedEdge.at<uchar>(cv::Point(x - 1, y))
					|| x + 1 < srcCroppedFingers.cols && srcCroppedEdge.at<uchar>(cv::Point(x + 1, y))) 
			{
				fingerTips[i] = cv::Point(x, y);
			}

			x += dx;
			y += dy;
		}
	}

	//TODO: Determen wich side is thumb side and wich side is pinky side
	//TODO: Determen the flexation of a finger
	//TODO: Determen the orientation of a finger

	//Create a image that shows the fingertip locations
	for (int x = 0; x < srcCroppedBGR.cols; ++x) {
		for (int y = 0; y < srcCroppedBGR.rows; ++y){
			for (int i = 0; i < fingerTips.size(); ++i) {
				cv::Point distance = fingerTips[i] - cv::Point(x, y);
				float length = pow(distance.x*distance.x + distance.y*distance.y, 0.5f);
				float drawRadius = 6;
				if (length < drawRadius && srcCroppedBinair.at<uchar>(cv::Point(x, y))) {
					cv::Vec3b drawColor(255, 0, 0);
					srcCroppedBGR.at<cv::Vec3b>(cv::Point(x, y)) = drawColor;
					break;
				}
			}
		}
	}


	//Generate histogram for pixel density in a circle around the wrist
	cv::Mat histogram(200, numberOfRays, CV_8UC1);
	for (int i = 0; i < histogram.cols; ++i) {
		for (int j = 0; j < histogram.rows; ++j) {
			histogram.at<uchar>(cv::Point(i, j)) = (histogram.rows - j < pixelCounts[i] * 5 ? 255 : 0);
		}
	}
	//Histogram that shows the wrist to finger directions
	cv::Mat histogramFinal = Mat::zeros(200, numberOfRays, CV_8UC1);
	for (int i = 0; i < peaks.size(); ++i) {
		for (int j = 0; j < histogramFinal.rows; ++j) {
			histogramFinal.at<uchar>(cv::Point(peaks[i], j)) = (histogramFinal.rows - j < pixelCounts[peaks[i]] * 5 ? 255 : 0);
		}
	}
	cv::imshow("finger reconision", srcCroppedBGR);
	cv::imshow("histogram", histogram);
	cv::imshow("histogram2", histogramFinal);

	//Wait until a key is pressed to kill the program
	cv::waitKey(0);

	return 0;
}
