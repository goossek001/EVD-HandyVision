#pragma once

#include <opencv2/opencv.hpp>

namespace cv {
	void threshold(const Mat* src, const Mat* dst, double lowerbound, double upperbound);
	void cvYCbCrThreshold(const Mat* src, Mat* dst,
		double Y_min, double Y_max,
		double Cb_min, double Cb_max,
		double Cr_min, double Cr_max);
	void detectAndDrawContour(const Mat* src, Mat* dst, int mode, int method, int minAreaThreshold = 16);
	Point getCenterOfMass(const Mat* src);
	std::vector<int> getHighestPeaks(int histogram[], int size, int nrPeaks);
}