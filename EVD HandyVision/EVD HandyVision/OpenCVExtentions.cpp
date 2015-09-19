#include "OpenCVExtentions.h"

namespace cv {
	void cvThreshHold(const Mat* src, const Mat* dst, double lowerBound, double upperBound) {
		inRange(*src, lowerBound, upperBound, *dst);
	}

	void cvYCbCrThreshHold(const Mat* src, Mat* dst,
		double Y_min, double Y_max,
		double Cb_min, double Cb_max,
		double Cr_min, double Cr_max) {
		//Split into channels
		Mat channels[3];
		split(*src, channels);

		//Apply thresholds
		cvThreshHold(&channels[0], &channels[0], Y_min, Y_max);
		cvThreshHold(&channels[1], &channels[1], Cb_min, Cb_max);
		cvThreshHold(&channels[2], &channels[2], Cr_min, Cr_max);

		//Combine threshold images
		channels[0].copyTo(*dst, channels[1]);
		channels[1].copyTo(*dst, *dst);
		channels[2].copyTo(*dst, *dst);
	}

	void DetectAndDrawContour(const Mat* src, Mat* dst, int mode, int method, int minAreaThreshold) {
		std::vector<cv::Vec4i> hierarchy;
		std::vector<std::vector<cv::Point> > contours;

		Mat src_copy;
		src->copyTo(src_copy);

		cv::findContours(src_copy, contours, hierarchy, mode, method);

		dst->create(src->size(), CV_8UC1);
		dst->setTo(cv::Scalar(0));

		for (size_t i = 0; i<contours.size(); ++i) 
			if (contourArea(contours[i]) >= minAreaThreshold)
				drawContours(*dst, contours, i, cv::Scalar(255), 1);
	}
}