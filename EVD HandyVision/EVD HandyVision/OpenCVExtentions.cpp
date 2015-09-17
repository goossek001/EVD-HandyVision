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
}