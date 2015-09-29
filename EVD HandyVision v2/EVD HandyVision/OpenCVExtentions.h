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
	std::vector<cv::RotatedRect> getBoundingBoxes(const Mat& src);
	std::vector<cv::Point> lineObjectIntersection(const Mat& src, float height, const std::vector<cv::Point>& intersections);
	cv::Point lineLineIntersection(cv::Point line1Point, cv::Point line1Direction, cv::Point line2Point, cv::Point line2Direction);
	void rotate(const cv::Mat& src, cv::Mat& dst, float angle);
	void rotatePoint(const cv::Mat& src, const cv::Point& srcPoint, cv::Point& dstPoint, float angle);
	float length(const cv::Point&);
	void rectMask(const cv::Mat& src, cv::Mat& dst, RotatedRect boundingRect);
}