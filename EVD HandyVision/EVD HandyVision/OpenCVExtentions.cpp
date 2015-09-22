#include "OpenCVExtentions.h"
#include <limits>

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

	Point getCenterOfMass(const Mat* src) {
		Moments moments = cv::moments(*src);
		return Point(moments.m10 / moments.m00, moments.m01 / moments.m00);
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

	std::vector<int> getHighestPeaks(int histogram[], int size, int nrPeaks) {
		std::vector<int> peaks;
		int index = 0;
		int last = 0;
		peaks.push_back(0);
		for (int i = 2; i < size; ++i) {
			if (histogram[i - 1] == histogram[i]) {
				index = i - 1;
			} else if (sign(histogram[i - 1] - histogram[i]) != sign(histogram[index] - histogram[i - 1])) {
				peaks.push_back(i - 1);

				index = i - 1;
			} else {
				last = i;
			}
		}
		peaks.push_back(last);

		int threshold = 1;
		while (peaks.size() > nrPeaks * 2 + 2) {
			int smallestPeak = std::numeric_limits<int>().max();
			for (int i = 2; i < peaks.size(); ++i) {
				int height = abs(histogram[peaks[i]] - histogram[peaks[i - 1]]);
				if (height <= threshold) {
					peaks.erase(peaks.begin() + i - 1, peaks.begin() + i);
					i -= 2;
				} else if (height < smallestPeak) {
					smallestPeak = height;
				}
			}
			threshold = smallestPeak;
		}

		struct Peak {
			int index;
			int height;
		};

		//sort
		std::vector<Peak> heights;
		for (int i = 0; i < peaks.size(); ++i) {
			int height;
			if (i == 0) {
				height = histogram[peaks[i]] - histogram[peaks[i + 1]];
			} else if (i == peaks.size() - 1) {
				height = histogram[peaks[i]] - histogram[peaks[i - 1]];
			} else {
				height = std::min(histogram[peaks[i]] - histogram[peaks[i - 1]], histogram[peaks[i]] - histogram[peaks[i + 1]]);
			}
			heights.push_back(Peak{ peaks[i], height });
		}
		for (int i = 0; i < heights.size(); ++i) {
			if (heights[i].height < 0) {
				heights[i] = heights[heights.size() - 1];
				heights.pop_back();
				--i;
			}
		}
		peaks.clear();
		for (int i = 0; i < heights.size(); ++i) {
			peaks.push_back(heights[i].index);
		}

		return peaks;
	}
}