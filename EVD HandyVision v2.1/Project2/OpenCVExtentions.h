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
	void HSVThreshold(const Mat& src, Mat& dst,
		double H_min, double H_max,
		double S_min, double S_max,
		double V_min, double V_max);
	Point getCenterOfMass(const Mat& src);
	void detectAndDrawContour(const Mat& src, Mat& dst, int mode, int method, int lineWidth = 1, int minAreaThreshold = 16);
	void fillHoles(const Mat& src, Mat& dst);
	std::vector<cv::RotatedRect> getBoundingBoxes(const Mat& src);
	void rotateImage(const cv::Mat& src, cv::Mat& dst, float angle);
	void applyRectangleMask(const cv::Mat& src, cv::Mat& dst, RotatedRect boundingRect);
	void getContour(cv::Mat& src, cv::Mat& dst);

}