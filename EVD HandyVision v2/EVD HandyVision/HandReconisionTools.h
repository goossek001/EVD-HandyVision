//***************************************************************************************
// Functions are specificly used for reconision skin and hands
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.00
//***************************************************************************************

#pragma once

#include <opencv2/opencv.hpp>
#include "Line.h"

using cv::Mat;

enum ThumbDirection {
	Left = 1,
	Right = -1
};
void BGRSkinColorFilter(const Mat& src, Mat& dst);
void YCbCrSkinColorFilter(const Mat& src, Mat& dst);
void getPalmCenter(const Mat& src, cv::Point& palmCenter, float& palmRadius);
void createPalmMask(const Mat& src, Mat& dst, cv::Point palmCenter, float palmRadius);
void findWrist(const Mat& src, cv::Line& wristOut, cv::Point palmCenter, float palmRadius);
void createFingerMask(const Mat& src, Mat& dst, Mat& palmMask, cv::Point wristCenter, cv::Point2f handOrientation);
int getFindThumb(const std::vector<cv::RotatedRect>& fingers, cv::Point palmCenter, float handAngle, ThumbDirection thumbDirection);
void findPalmLine(const Mat& srcBinair, cv::Line& palmLineOut, cv::Line wristLine, float palmRadius, cv::Point2f handOrientation, bool isThumbVisible);
void labelFingers(const std::vector<cv::RotatedRect>& boundingBoxesFingers, const cv::Point& wristCenter, const cv::Point& handOrientation
	, cv::Line palmLine, int thumbIndex, int& indexFingerIndexOut, int& middleFingerIndexOut, int& ringFingerIndexOut, int& pinkyIndexOut);