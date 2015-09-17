#pragma once

#include <opencv2/opencv.hpp>

namespace cv {
	void cvThreshHold(const Mat* src, const Mat* dst, double lowerBound, double upperBound);
	void cvYCbCrThreshHold(const Mat* src, Mat* dst,
		double Y_min, double Y_max,
		double Cb_min, double Cb_max,
		double Cr_min, double Cr_max);
	void DetectAndDrawContour(const Mat* src, Mat* dst, int mode, int method, int minAreaThreshold = 16);
}