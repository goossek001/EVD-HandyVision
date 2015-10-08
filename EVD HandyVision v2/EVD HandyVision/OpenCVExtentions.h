//***************************************************************************************
// Functions that manipulate images
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.00
//***************************************************************************************

#pragma once

#include <opencv2/opencv.hpp>

namespace cv {
	void threshold(const Mat& src, const Mat& dst, double lowerbound, double upperbound);
	void YCbCrThreshold(const Mat& src, Mat& dst,
		double Y_min, double Y_max,
		double Cb_min, double Cb_max,
		double Cr_min, double Cr_max);
	void BGRThreshold(const Mat& src, Mat& dst,
		double R_min, double R_max,
		double G_min, double G_max,
		double B_min, double B_max);
	Point getCenterOfMass(const Mat& src);
	void detectAndDrawContour(const Mat& src, Mat& dst, int mode, int method, int lineWidth = 1, int minAreaThreshold = 16);
	void fillHoles(const Mat& src, Mat& dst);
	std::vector<cv::RotatedRect> getBoundingBoxes(const Mat& src);
	void rotateImage(const cv::Mat& src, cv::Mat& dst, float angle);
	void applyRectangleMask(const cv::Mat& src, cv::Mat& dst, RotatedRect boundingRect);
}