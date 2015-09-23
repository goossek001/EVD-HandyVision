#include "HandReconisionTools.h"

#include "OpenCVExtentions.h"

#define PI 3.14159265359

/**
	A filter create a binair image that has seperated skin and background
	@param src: a YCbCr image
	@param dst: a 8 bit binair image. Skin will have value 1 and non-skin value 0
*/
void YCbCrSkinColorFilter(const Mat& src, Mat& dst) {
	cv::cvYCbCrThreshold(src, dst, 0, 255, 77, 127, 133, 173);
} 

/**
	Sort two blobs on contour area
*/
bool sortBlobs(std::vector<cv::Point> first, std::vector<cv::Point> second) {
	return contourArea(first) > contourArea(second);
}

/**
	Find fingers by looking for parralel lines that are distance 'fingerThickness' appart
	@param src: hand edges stored in a 8 bit binair image
	@param dst: a 8 bit binair image. Finger areas will have value 1 and non-fingers value 0
*/
void findFingers(const Mat& src, Mat& dst, const Mat& mask, float fingerThickness) {
	int N = (int)round(fingerThickness / 4);

	//Create the kernel
	Mat kernel;
	kernel.create(N * 6 + 1, N * 6 + 1, CV_32F);
	for (int i = 0; i < kernel.cols; ++i) {
		for (int j = 0; j < kernel.rows; ++j) {
			int distance = (int)sqrt(pow(i - N*3, 2) + pow(j - N*3, 2));
			if (distance > 3 * N) {
				kernel.at<float>(cv::Point(i,j)) = 0;
			} else if (distance > N * 2) {
				kernel.at<float>(cv::Point(i, j)) = 1;
			} else if (distance > N) {
				kernel.at<float>(cv::Point(i, j)) = 0;
			} else {
				kernel.at<float>(cv::Point(i, j)) = -1;
			}
		}
	}
	kernel /= (float)(kernel.cols * kernel.rows);

	//Apply kernel in a convolution operation
	int ddepth = -1;
	filter2D(src, dst, ddepth, kernel, cv::Point(-1, -1), 0, cv::BORDER_ISOLATED);
	float thresh = 5*sqrtf((float)(10*N));
	cv::threshold(dst, dst, thresh, 2 * thresh);

	//Apply mask
	cv::bitwise_and(dst, mask, dst);

	//Close seperated parts of a finger
	Mat dilatingKernel = Mat::ones(3, 3, CV_8UC1);
	dilatingKernel.at<uchar>(cv::Point(1, 1)) = 0;
	cv::morphologyEx(dst, dst, cv::MORPH_DILATE, dilatingKernel);
}

void removeArm(const Mat& src, Mat& dst, cv::Point fingerCenterOfMass, float fingerThickness) {
	float cropRadius = 10 * fingerThickness;
	cv::Mat mask = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	cv::circle(mask, fingerCenterOfMass, cropRadius, cv::Scalar(255), -1, 8, 0);
	cv::bitwise_and(src, mask, dst);
}

void getPalmCenter(const Mat& src, cv::Point& palmCenter, float& palmRadius) {
	Mat dst, dstNormalized;
	cv::distanceTransform(src, dst, CV_DIST_L2, 3, CV_32F);

	float highestValue = 0;
	int count = 0;
	long xtotal = 0;
	long ytotal = 0;
	for (int x = 0; x < dst.cols; ++x) {
		for (int y = 0; y < dst.rows; ++y) {
			if (dst.at<float>(cv::Point(x, y)) >= highestValue) {
				if (dst.at<float>(cv::Point(x, y)) > highestValue) {
					highestValue = dst.at<float>(cv::Point(x, y));
					count = 1;
					xtotal = x;
					ytotal = y;
				} else {
					count++;
					xtotal += x;
					ytotal += y;
				}
			}
		}
	}

	palmCenter.x = xtotal / count;
	palmCenter.y = ytotal / count;
	palmRadius = dst.at<float>(palmCenter);
}

bool hasLotsOfFriends(const Mat& src, cv::Point pos) {
	bool lotsOfFriends = true;
	for (int x = -1; x <= 1; x += 2) {
		for (int y = -1; y <= 1; y += 2) {
			cv::Point friendPos(pos.x + x, pos.y + y);
			if (friendPos.x >= 0 && friendPos.x < src.cols &&
				friendPos.y >= 1 && friendPos.y < src.rows &&
				!src.at<uchar>(friendPos)) 
			{
				return false;
			}
		}
	}
	return true;
}

void findLargestGap(const Mat& src, cv::Point& pos1, cv::Point& pos2, cv::Point center, float maxRadius, float angleStart, float angleEnd) {
	float largestGap = -1;

	cv::Point previousPoint;
	float previousAngle;

	float deltaAngle = PI / 180.0f;
	for (float angle = angleStart; angle < angleEnd; angle += deltaAngle) {
		for (int radius = 0; radius < maxRadius; ++radius) {
			cv::Point pixelPos = center + cv::Point(cos(angle) * radius, sin(angle) * radius);
			if (!hasLotsOfFriends(src, pixelPos)) {
				if (largestGap == -1) {
					largestGap = 0;
				} else if (std::abs(previousAngle - angle) / deltaAngle - 1 > largestGap) {
					largestGap = std::abs(previousAngle - angle) / deltaAngle - 1;
					pos1 = previousPoint;
					pos2 = pixelPos;
				}
				previousPoint = pixelPos;
				previousAngle = angle;

				break;
			}
		}
	}
}


void findWrist(const Mat& src, cv::Point& wristLeft, cv::Point& wristRight, cv::Point palmCenter, float palmRadius) {
	findLargestGap(src, wristLeft, wristRight, palmCenter, 1.75*palmRadius, 0, 3.0f*PI);

	float asdf = 0.5f * PI;
	float minAngle = std::atan2(wristLeft.y - palmCenter.y, wristRight.x - palmCenter.x) - asdf;
	float maxAngle = std::atan2(wristRight.y - palmCenter.y, wristRight.x - palmCenter.x) + asdf;
	findLargestGap(src, wristLeft, wristRight, palmCenter, 1.2*palmRadius, minAngle, maxAngle);
}

void getPalmMask(const Mat& src, Mat& dst, cv::Point palmCenter, float palmRadius) {
	cv::Mat circleMask = cv::Mat::zeros(src.rows, src.cols, src.type());
	circle(circleMask, palmCenter, 1.2f * palmRadius, cv::Scalar(255), -1, 8, 0);

	dst.create(src.size(), src.type());
	dst.setTo(cv::Scalar(0));
	src.copyTo(dst, circleMask);
}