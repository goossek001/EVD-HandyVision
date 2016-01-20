//***************************************************************************************
// A collection of mathmaticly functions
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.01
//***************************************************************************************

#include "Math.h"
#include "Line.h"

namespace math {
	/**
	The signature of a integer
	@param val:
	@return:		-1 for negative values and 1 for all other values
	*/
	int sign(int val) {
		if (val >= 0) {
			return 1;
		} else if (val < 0) {
			return 0;
		}
	}

	/**
	Calculates the length of a vector, using Pythagorean theorem
	@param vector:
	@return:		The length of vector
	*/
	float length(const cv::Point& vector) {
		return pow(vector.x*vector.x + vector.y*vector.y, 0.5f);
	}

	float vlength(const vision::Point& vector) {
		return pow(vector.x*vector.x + vector.y*vector.y, 0.5f);
	}

	/**
	Calculates the crossproduct of two vectors
	@param v1:
	@param v2:
	@return:		the crossproduct 'v1 X v2'
	*/
	float cross(cv::Point v1, cv::Point v2) {
		return (v1.x*v2.y) - (v1.y*v2.x);
	}

	cv::Point operator*(const cv::Point& p, cv::Mat M) {
		cv::Mat_<double> src(3/*rows*/, 1 /* cols */);

		src(0, 0) = p.x;
		src(1, 0) = p.y;
		src(2, 0) = 1.0;

		cv::Mat_<double> dst = M*src; //USE MATRIX ALGEBRA 
		return cv::Point(dst(0, 0), dst(1, 0));
	}

	/**
	Rotate a point around the center of the image
	@param src:			A image with no type restrictions
	@param srcPoint:	The point that will get rotated by the algoritm
	@param dstPoint:	The output channel for the rotated point
	@param angle:		The rotation of the point in radians
	*/
	void rotatePoint(const vision::Mat& src, const vision::Point& srcPoint, vision::Point& dstPoint, float angle) {
		int len = std::max(src.cols, src.rows);
		vision::Point2f pt(len / 2., len / 2.);
		vision::Mat R = vision::getRotationMatrix2D(pt, angle / PI * 180);
		vision::warpAffine(srcPoint, dstPoint, R);
	}

	/**
	Rotate a line around the center of the image
	@param src:			A image with no type restrictions
	@param srcPoint:	The line that will get rotated by the algoritm
	@param dstPoint:	The output channel for the rotated line
	@param angle:		The rotation of the line in radians
	*/
	void rotateLine(const vision::Mat& src, vLine& srcLine, vLine& dstLine, float angle) {
		vision::Point lineEnd = srcLine.vlineEnd();
		rotatePoint(src, lineEnd, lineEnd, angle);
		rotatePoint(src, srcLine.vposition, dstLine.vposition, angle);
		dstLine.vdirection = lineEnd - dstLine.vposition;
	}

	/**
	Calculate the intersecting point of two lines
	@param l1:
	@param l2:
	@return:	The intersecting point of the two lines
	*/
	cv::Point lineLineIntersection(Line l1, Line l2) {
		float u = cross(l2.position - l1.position, l1.direction) / cross(l1.direction, l2.direction);
		return l2.position + u * l2.direction;
	}

}