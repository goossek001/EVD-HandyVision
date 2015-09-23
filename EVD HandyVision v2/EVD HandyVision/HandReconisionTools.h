#pragma once

#include <opencv2/opencv.hpp>

using cv::Mat;

void YCbCrSkinColorFilter(const Mat& src, Mat& dst);
void findFingers(const Mat& src, Mat& dst, const Mat& mask, float fingerThickness);
void getPalmCenter(const Mat& src, cv::Point& palmCenter, float& palmRadius);
void getPalmMask(const Mat& src, Mat& dst, cv::Point palmCenter, float palmRadius);
void findWrist(const Mat& src, cv::Point& wristLeft, cv::Point& wristRight, cv::Point palmCenter, float palmRadius);