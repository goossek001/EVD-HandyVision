#pragma once

#include <opencv2/opencv.hpp>

namespace cv {
	void threshold(const Mat& src, const Mat& dst, double lowerbound, double upperbound);
	void cvYCbCrThreshold(const Mat& src, Mat& dst,
		double Y_min, double Y_max,
		double Cb_min, double Cb_max,
		double Cr_min, double Cr_max);
	Point getCenterOfMass(const Mat& src);
	void detectAndDrawContour(const Mat& src, Mat& dst, int mode, int method, int lineWidth = 1, int minAreaThreshold = 16);
	void fill(const Mat& src, Mat& dst);
}