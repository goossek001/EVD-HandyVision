#include "HandReconisionTools.h"

#include "OpenCVExtentions.h"

/**
	A filter create a binair image that has seperated skin and background
	@param src: a YCbCr image
	@param dst: a 8 bit binair image. Skin will have value 1 and non-skin value 0
*/
void YCbCrSkinColorFilter(const Mat* src, Mat* dst) {
	cv::cvYCbCrThreshold(src, dst, 0, 255, 77, 127, 133, 173);
} 

/**
	Sort two blobs on contour area
*/
bool sortBlobs(std::vector<cv::Point> first, std::vector<cv::Point> second) {
	return contourArea(first) > contourArea(second);
}

/**
	Find fingers by looking for parralel lines that are distance 'fingerThickness' appart
	@param src: hand edges stored in a 8 bit binair image
	@param dst: a 8 bit binair image. Finger areas will have value 1 and non-fingers value 0
*/
void findFingers(const Mat* src, Mat* dst, float fingerThickness) {
	int N = (int)round(fingerThickness / 4);

	//Create the kernel
	Mat kernel;
	kernel.create(N * 6 + 1, N * 6 + 1, CV_32F);
	for (int i = 0; i < kernel.cols; ++i) {
		for (int j = 0; j < kernel.rows; ++j) {
			int distance = (int)sqrt(pow(i - N*3, 2) + pow(j - N*3, 2));
			if (distance > 3 * N) {
				kernel.col(i).row(j) = 0;
			} else if (distance > N * 2) {
				kernel.col(i).row(j) = 1;
			} else if (distance > N) {
				kernel.col(i).row(j) = 0;
			} else {
				kernel.col(i).row(j) = -1;
			}
		}
	}
	kernel /= (float)(kernel.cols * kernel.rows);

	//Apply kernel in a convolution operation
	int ddepth = -1;
	Mat tmp;
	filter2D(*src, tmp, ddepth, kernel, cv::Point(-1, -1), 0, cv::BORDER_ISOLATED);
	float thresh = 5*sqrtf(10*N);
	cv::threshold(&tmp, &tmp, thresh, 2 * thresh);

	//Close seperated parts of a finger
	Mat dilationKernel = Mat::ones(3, 3, CV_8UC1);
	dilationKernel.col(1).row(1) = 0;
	cv::morphologyEx(tmp, tmp, cv::MORPH_CLOSE, dilationKernel);

	//Detect blobs
	std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(tmp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//Keep the 5 blobs with the biggest perimeter
	std::sort(contours.begin(), contours.end(), sortBlobs);
	contours.resize(std::min(5, (int)contours.size()));

	//Draw the 5 blobs to the destination image
	dst->create(tmp.size(), CV_8UC1);
	dst->setTo(cv::Scalar(0));
	for (size_t i = 0; i<contours.size(); ++i)
		drawContours(*dst, contours, (int)i, cv::Scalar(255), -1);
}