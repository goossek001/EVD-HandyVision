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
void findFingers(const Mat* src, Mat* dst, const Mat* mask, float fingerThickness) {
	int N = (int)round(fingerThickness / 4);

	//Create the kernel
	Mat kernel;
	kernel.create(N * 6 + 1, N * 6 + 1, CV_32F);
	for (int i = 0; i < kernel.cols; ++i) {
		for (int j = 0; j < kernel.rows; ++j) {
			int distance = (int)sqrt(pow(i - N*3, 2) + pow(j - N*3, 2));
			if (distance > 3 * N) {
				kernel.at<float>(cv::Point(i,j)) = 0;
			} else if (distance > N * 2) {
				kernel.at<float>(cv::Point(i, j)) = 1;
			} else if (distance > N) {
				kernel.at<float>(cv::Point(i, j)) = 0;
			} else {
				kernel.at<float>(cv::Point(i, j)) = -1;
			}
		}
	}
	kernel /= (float)(kernel.cols * kernel.rows);

	//Apply kernel in a convolution operation
	int ddepth = -1;
	filter2D(*src, *dst, ddepth, kernel, cv::Point(-1, -1), 0, cv::BORDER_ISOLATED);
	float thresh = 5*sqrtf((float)(10*N));
	cv::threshold(dst, dst, thresh, 2 * thresh);

	//Apply mask
	cv::bitwise_and(*dst, *mask, *dst);

	//Close seperated parts of a finger
	Mat dilatingKernel = Mat::ones(3, 3, CV_8UC1);
	dilatingKernel.at<uchar>(cv::Point(1, 1)) = 0;
	cv::morphologyEx(*dst, *dst, cv::MORPH_DILATE, dilatingKernel);
}

void removeArm(const Mat* src, Mat* dst, cv::Point fingerCenterOfMass, float fingerThickness) {
	float cropRadius = 10 * fingerThickness;
	cv::Mat mask = cv::Mat::zeros(src->rows, src->cols, CV_8UC1);
	cv::circle(mask, fingerCenterOfMass, cropRadius, cv::Scalar(255), -1, 8, 0);
	cv::bitwise_and(*src, mask, *dst);
}

cv::Point getHandCenter(const Mat* src) {
	Mat dst(*src);
	cv::detectAndDrawContour(&dst, &dst, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	return cv::getCenterOfMass(&dst);
}