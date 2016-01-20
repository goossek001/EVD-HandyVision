//***************************************************************************************
// Functions are specificly used for reconision skin and hands
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.00
//***************************************************************************************

#pragma once

#include <opencv2/opencv.hpp>
#include "Line.h"
#include <map>
#include "VisionOperators.h"
#include "OpenCVExtentions.h"

using vision::Mat;

enum ThumbDirection {
	Left = 1,
	Right = -1
};
enum GestureType {
	DutchCounting,
	COUNT
};

void initHashTable();
int GenerateHashKey(bool fingers[5]);

void biggestColorBlob(const Mat& src, Mat& dst, const Mat& mask = Mat());
void adaptiveHSVSkinColorFilter(const Mat& src, Mat& dst,
	int H_min, int H_max,
	int S_min, int S_max,
	int V_min, int V_max,
	int S_size, int V_size);
void CannyHandFilter(const Mat& src, Mat& dst);
void getPalmCenter(const Mat& src, vision::Point& palmCenter, float& palmRadius);
void createPalmMask(const Mat& src, Mat& dst, vision::Point palmCenter, float palmRadius);
void findWrist(const Mat& src, vLine& wristOut, bool& foundWrist, vision::Point palmCenter, float palmRadius);
void createFingerMask(const Mat& src, Mat& dst, Mat& palmMask, vision::Point wristCenter, vision::Point2f handOrientation);
int getFindThumb(const std::vector<cv::RotatedRect>& fingers, cv::Point palmCenter, float handAngle, ThumbDirection thumbDirection);
void findPalmLine(const Mat& srcBinair, vLine& palmLineOut, bool& foundPalm, vLine wristLine, float palmRadius, vision::Point2f handOrientation, bool isThumbVisible);
void labelFingers(std::vector<cv::RotatedRect>& fingersIn, cv::RotatedRect* (&fingersOut)[5], const cv::Point& wristCenter
	, const cv::Point& handOrientation, Line palmLine);
void areFingersStretched(cv::RotatedRect* fingers[5], bool(&out)[5], float palmRadius);
void displayFingers(const cv::Mat& img, cv::RotatedRect* fingers[5]);
std::string deteremenGesture(GestureType gestureType, bool fingers[5]);