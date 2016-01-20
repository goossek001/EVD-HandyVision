//***************************************************************************************
// Functions that manipulate images
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.00
//***************************************************************************************

#pragma once

#include "VisionOperators.h"

namespace vision {
	void HSVThreshold(const Mat& src, Mat& dst,
		double H_min, double H_max,
		double S_min, double S_max,
		double V_min, double V_max);
	//vision::Point getCenterOfMass(const Mat& src);
	void fillHoles(const Mat& src, Mat& dst);
	//std::vector<vision::Rect_obb> getBoundingBoxes(const Mat& src);
	std::vector<cv::RotatedRect> getBoundingBoxes(const cv::Mat& src);
	void rotateImage(const cv::Mat& src, cv::Mat& dst, float angle);
	void rotateImage(const vision::Mat& src, vision::Mat& dst, float angle);
	void applyRectangleMask(const Mat& src, Mat& dst, vision::Rect_obb boundingRect);
	void getBiggestBlob(const Mat& src, Mat& dst);
}