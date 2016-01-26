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

using cv::Mat;

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
void adaptiveHSVSkinColorFilter(const vision::Mat& src, vision::Mat& dst,
	int H_min, int H_max,
	int S_min, int S_max,
	int V_min, int V_max,
	int S_size, int V_size);
void CannyHandFilter(const Mat& src, Mat& dst);
void getPalmCenter(const vision::Mat& src, vision::Point& palmCenter, float& palmRadius);
void createPalmMask(const vision::Mat& src, vision::Mat& dst, vision::Point palmCenter, float palmRadius);
void findWrist(const vision::Mat& src, vision::Line& wristOut, bool& foundWrist, vision::Point palmCenter, float palmRadius);
void createFingerMask(const vision::Mat& src, vision::Mat& dst, vision::Mat& palmMask, vision::Point wristCenter, vision::Point2f handOrientation);
int findThumb(const std::vector<vision::Rect_obb>& fingers, vision::Point palmCenter, float handAngle, ThumbDirection thumbDirection);
void findPalmLine(const vision::Mat& srcBinair, vision::Line& palmLineOut, bool& foundPalm, vision::Line wristLine, float palmRadius, vision::Point2f handOrientation, bool isThumbVisible);
void labelFingers(std::vector<vision::Rect_obb>& fingersIn, vision::Rect_obb* (&fingersOut)[5], const vision::Point& wristCenter
	, const vision::Point& handOrientation, const vision::Line palmLine);
void areFingersStretched(vision::Rect_obb* fingers[5], bool(&out)[5], float palmRadius);
std::string deteremenGesture(GestureType gestureType, bool fingers[5]);

void drawFingers(const cv::Mat& src, cv::Mat& dst, const std::vector<vision::Rect_obb>& fingers, vision::Line palmLine, vision::Point wristCenter);