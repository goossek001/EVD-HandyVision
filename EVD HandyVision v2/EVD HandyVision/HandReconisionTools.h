#pragma once

#include <opencv2/opencv.hpp>

using cv::Mat;

enum ThumbDirection {
	Left = 1,
	Right = -1
};

void YCbCrSkinColorFilter(const Mat& src, Mat& dst);
void findFingers(const Mat& src, Mat& dst, const Mat& mask, float fingerThickness);
void getPalmCenter(const Mat& src, cv::Point& palmCenter, float& palmRadius);
void getPalmMask(const Mat& src, Mat& dst, cv::Point palmCenter, float palmRadius);
void findWrist(const Mat& src, cv::Point& wristLeft, cv::Point& wristRight, cv::Point palmCenter, float palmRadius);
void getFingerMask(const Mat& src, Mat& dst, Mat& palmMask, cv::Point wristCenter, cv::Point2f handOrientation);
int getFindThumb(const std::vector<cv::RotatedRect>& fingers, cv::Point palmCenter, float handAngle, ThumbDirection thumbDirection);
void getPalmLine(const Mat& srcBinair, cv::Point& palmLineStart, cv::Point& palmLineEnd, cv::Point wristLeft, cv::Point wristRight, cv::Point2f handOrientation, bool isThumbVisible);