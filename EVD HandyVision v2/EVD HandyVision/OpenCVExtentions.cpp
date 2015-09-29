#include "OpenCVExtentions.h"
#include "Math.h"
#include <limits>

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

	/**
		Rotate a image around its center 
		@param src:			A image with no type restrictions
		@param dst			The output channel for the rotated image. This image will have the same type and parent as the param 'src'
		@param angle:		The rotation of the image in radians
	*/
	void rotateImage(const cv::Mat& src, cv::Mat& dst, float angle) {
		int len = std::max(src.cols, src.rows);
		cv::Point2f pt(len / 2., len / 2.);
		cv::Mat r = cv::getRotationMatrix2D(pt, angle*180/PI, 1.0);

		cv::warpAffine(src, dst, r, cv::Size(len, len));
	}

	void applyRectangleMask(const cv::Mat& src, cv::Mat& dst, RotatedRect boundingRect) {
		Mat rectMask = Mat::zeros(src.size(), src.type());
		Point2f vertices[4];
		boundingRect.points(vertices);
		for (int i = 0; i < 4; i++)
			line(rectMask, vertices[i], vertices[(i + 1) % 4], Scalar(255));
		fill(rectMask, rectMask);

		src.copyTo(dst, rectMask);
	}
}