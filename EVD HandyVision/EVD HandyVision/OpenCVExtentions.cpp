#include "OpenCVExtentions.h"

namespace cv {
	/**
		Apply a threshold with a lower- and upperbound
		@param src: a 1 channel 8 bit image
		@param dst: a 8 bit binair image
	*/
	void threshold(const Mat* src, const Mat* dst, double lowerbound, double upperbound) {
		inRange(*src, lowerbound, upperbound, *dst);
	}

	/**
		Apply a threshold on a YCbCr image
		@param src: a 3 channel YCbCr image
		@param dst: a 8 bit binair image
	*/
	void cvYCbCrThreshold(const Mat* src, Mat* dst,
		double Y_min, double Y_max,
		double Cb_min, double Cb_max,
		double Cr_min, double Cr_max) {
		//Split into channels
		Mat channels[3];
		split(*src, channels);

		//Apply thresholds
		threshold(&channels[0], &channels[0], Y_min, Y_max);
		threshold(&channels[1], &channels[1], Cb_min, Cb_max);
		threshold(&channels[2], &channels[2], Cr_min, Cr_max);

		//Combine threshold images
		cv::bitwise_and(channels[0], channels[1], *dst, channels[2]);
	}

	/**
		Detect and the contour of objects
		@param src: a 8 bit binair image
		@param dst: a 8 bit binair image
	*/
	void detectAndDrawContour(const Mat* src, Mat* dst, int mode, int method, int minAreaThreshold) {
		std::vector<cv::Vec4i> hierarchy;
		std::vector<std::vector<cv::Point> > contours;

		Mat srcCopy;
		src->copyTo(srcCopy);

		cv::findContours(srcCopy, contours, hierarchy, mode, method);

		dst->create(src->size(), CV_8UC1);
		dst->setTo(cv::Scalar(0));

		for (size_t i = 0; i<contours.size(); ++i)
			//TODO: the filter should be placed in this function or rename the function
			//TODO: remove big skin areas that arent hands
			if (contourArea(contours[i]) >= minAreaThreshold)
				drawContours(*dst, contours, i, cv::Scalar(255), 1);
	}
}