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
	using cv::Mat;
	using cv::Point;
	using cv::Line;

	int sign(int val);

	float length(const Point& vector);
	float length(const vision::Point& vector);
	float cross(cv::Point v1, cv::Point v2);

	void rotatePoint(const Mat& src, const Point& srcPoint, Point& dstPoint, float angle);
	void rotatePoint(const vision::Mat& src, const vision::Point& srcPoint, vision::Point& dstPoint, float angle);
	void rotateLine(const Mat& src, Line& srcLine, Line& dstLine, float angle);
	void rotateLine(const vision::Mat& src, vision::Line& srcLine, vision::Line& dstLine, float angle);
	std::vector<Point> horizontalLineObjectIntersection(const Mat& src, int height);
	std::vector<vision::Point> horizontalLineObjectIntersection(const vision::Mat& src, int height);
	Point lineLineIntersection(Line l1, Line l2);
}