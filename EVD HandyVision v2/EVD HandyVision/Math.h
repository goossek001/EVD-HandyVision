//***************************************************************************************
// A collection of mathmaticly constants and functions
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.01
//***************************************************************************************

#include "Line.h"

#define PI 3.14159265359
#define Infinity 9999999

namespace math {
	using cv::Mat;
	using cv::Point;
	using cv::Line;

	int sign(int val);

	float length(const Point& vector);
	float cross(cv::Point v1, cv::Point v2);

	void rotatePoint(const Mat& src, const Point& srcPoint, Point& dstPoint, float angle);
	void rotateLine(const Mat& src, Line& srcLine, Line& dstLine, float angle);
	std::vector<Point> horizontalLineObjectIntersection(const Mat& src, int height);
	Point lineLineIntersection(Line l1, Line l2);
}