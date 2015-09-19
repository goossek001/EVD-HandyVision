#pragma once

#include <opencv2/opencv.hpp>

using cv::Mat;

void YCbCrSkinColorFilter(const Mat* src, Mat* dst);
void findFingers(const Mat* src, Mat* dst, const Mat* mask, float fingerThickness);