//***************************************************************************************
// A collection of mathmaticly constants and functions
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.01
//***************************************************************************************

#include "Line.h"
#include "VisionOperators.h"

#define PI 3.14159265359
#define Infinity 9999999

namespace math {
	int sign(int val);
// OPENCV
	float length(const cv::Point& vector);
	float cross(cv::Point v1, cv::Point v2);

	void rotatePoint(const vision::Mat& src, const vision::Point& srcPoint, vision::Point& dstPoint, float angle);
	void rotateLine(const vision::Mat& src, vLine& srcLine, vLine& dstLine, float angle);
	cv::Point lineLineIntersection(Line l1, Line l2);
	std::vector<vision::Point> horizontalLineObjectIntersection(const vision::Mat& src, int height);
	float vlength(const vision::Point& vector);

}