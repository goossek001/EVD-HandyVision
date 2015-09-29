#include "OpenCVExtentions.h"
#include <limits>

#define PI 3.14159265359

namespace cv {
	/**
		Apply a threshold with a lower- and upperbound
		@param src: a 1 channel 8 bit image
		@param dst: a 8 bit binair image
		*/
	void threshold(const Mat& src, const Mat& dst, double lowerbound, double upperbound) {
		inRange(src, lowerbound, upperbound, dst);
	}

	/**
		Apply a threshold on a YCbCr image
		@param src: a 3 channel YCbCr image
		@param dst: a 8 bit binair image
		*/
	void cvYCbCrThreshold(const Mat& src, Mat& dst,
		double Y_min, double Y_max,
		double Cb_min, double Cb_max,
		double Cr_min, double Cr_max) {
		//Split into channels
		Mat channels[3];
		split(src, channels);

		//Apply thresholds
		threshold(channels[0], channels[0], Y_min, Y_max);
		threshold(channels[1], channels[1], Cb_min, Cb_max);
		threshold(channels[2], channels[2], Cr_min, Cr_max);

		//Combine threshold images
		cv::bitwise_and(channels[0], channels[1], dst, channels[2]);
	}
	/**
	Detect and the contour of objects
	@param src: a 8 bit binair image
	@param dst: a 8 bit binair image
	*/
	void detectAndDrawContour(const Mat& src, Mat& dst, int mode, int method, int lineWidth , int minAreaThreshold) {
		std::vector<cv::Vec4i> hierarchy;
		std::vector<std::vector<cv::Point> > contours;

		Mat srcCopy;
		src.copyTo(srcCopy);

		cv::findContours(srcCopy, contours, hierarchy, mode, method);

		dst.create(src.size(), CV_8UC1);
		dst.setTo(cv::Scalar(0));

		for (size_t i = 0; i<contours.size(); ++i)
			//TODO: the filter should be placed in this function or rename the function
			//TODO: remove big skin areas that arent hands
		if (contourArea(contours[i]) >= minAreaThreshold)
			drawContours(dst, contours, i, cv::Scalar(255), lineWidth);
	}

	void fill(const Mat& src, Mat& dst) {
		detectAndDrawContour(src, dst, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, -1);
	}

	std::vector<cv::RotatedRect> getBoundingBoxes(const Mat& src) {
		int minAreaThreshold = 16;
		std::vector<cv::Vec4i> hierarchy;
		std::vector<std::vector<cv::Point> > contours;

		Mat srcCopy;
		src.copyTo(srcCopy);

		std::vector<cv::RotatedRect> boundingBoxes;
		cv::findContours(srcCopy, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		for (int i = 0; i < contours.size(); ++i)
			if (contourArea(contours[i]) >= minAreaThreshold)
				boundingBoxes.push_back(cv::minAreaRect(contours[i]));

		return boundingBoxes;
	}

	int sign(int val) {
		if (val > 0) {
			return 1;
		} else if (val < 0) {
			return -1;
		} else {
			return 0;
		}
	}

	std::vector<cv::Point> lineObjectIntersection(const Mat& src, float height, const std::vector<cv::Point>& intersections) {
		std::vector<cv::Point> out(intersections);
		int x = 0;

		int previousVal = 0;
		while (x < src.cols) {
			if ((src.at<uchar>(cv::Point(x, height)) == 0) != (previousVal == 0)) {

				if (previousVal == 0)
					out.push_back(cv::Point(x, height));
				else
					out.push_back(cv::Point(x - 1, height));

				previousVal = src.at<uchar>(cv::Point(x, height));
			}

			++x;
		}
		if (intersections.size() % 2)
			out.push_back(cv::Point(src.cols-1, height));
		return out;
	}

	void rotate(const cv::Mat& src, cv::Mat& dst, float angle) {
		int len = std::max(src.cols, src.rows);
		cv::Point2f pt(len / 2., len / 2.);
		cv::Mat r = cv::getRotationMatrix2D(pt, angle*180/PI, 1.0);

		cv::warpAffine(src, dst, r, cv::Size(len, len));
	}

	void rotatePoint(const cv::Mat& src, const cv::Point& srcPoint, cv::Point& dstPoint, float angle) {
		int len = std::max(src.cols, src.rows);
		cv::Point2f pt(len / 2., len / 2.);

		cv::Point2f temp = (cv::Point2f) srcPoint - pt;
		temp.x = std::cos(angle)*temp.x - std::sin(angle)*temp.y;
		temp.y = std::sin(angle)*temp.x + std::cos(angle)*temp.y;
		temp += pt;

		dstPoint = (cv::Point) temp;
	}

	float cross(cv::Point v1, cv::Point v2) {
		return (v1.x*v2.y) - (v1.y*v2.x);
	}

	cv::Point lineLineIntersection(cv::Point line1Point, cv::Point line1Direction, cv::Point line2Point, cv::Point line2Direction) {
		float u = cross(line2Point - line1Point, line1Direction) / cross(line1Direction, line2Direction);
		return line2Point + u * line2Direction;
	}

	float length(const cv::Point& point) {
		return pow(point.x*point.x + point.y*point.y, 0.5f);
	}

	void rectMask(const cv::Mat& src, cv::Mat& dst, RotatedRect boundingRect) {
		Mat rectMask = Mat::zeros(src.size(), src.type());
		Point2f vertices[4];
		boundingRect.points(vertices);
		for (int i = 0; i < 4; i++)
			line(rectMask, vertices[i], vertices[(i + 1) % 4], Scalar(255));
		fill(rectMask, rectMask);

		src.copyTo(dst, rectMask);
	}
}