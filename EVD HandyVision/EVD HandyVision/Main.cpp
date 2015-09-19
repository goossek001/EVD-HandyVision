#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "OpenCVExtentions.h"
#include "HandReconisionTools.h"

#include <Windows.h>

int main(int argc, char** argb) {
	cv::Mat srcRGB, srcYCrCb, srcGray, srcBinair, srcOuterEdge, srcCannyEdge, srcFullEdge, srcFingers;
	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	char* window_name = "Laplace Demo";

	//open image
	srcRGB = cv::imread("img1.jpg");
	if (!srcRGB.data)
		return -1;

	//HSV skin color filter
	cv::cvtColor(srcRGB, srcYCrCb, CV_RGB2YCrCb);
	YCbCrSkinColorFilter(&srcYCrCb, &srcBinair);

	//TODO: remove big skin areas that arent hands
	//TODO: clip the img

	//Detect outer contour
	detectAndDrawContour(&srcBinair, &srcOuterEdge, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//Apply canny filter
	cv::cvtColor(srcRGB, srcGray, CV_RGB2GRAY);
	cv::Canny(srcGray, srcCannyEdge, 120, 255);

	//Combine the outercontour with the canny edge
	cv::bitwise_or(srcOuterEdge, srcCannyEdge, srcFullEdge);

	//TODO: estimate finger size
	float fingerThickness = 8;

	findFingers(&srcFullEdge, &srcFingers, fingerThickness);

	//Display the result
	cv::imshow(window_name, srcFingers);

	//Wait until a key is pressed to kill the program
	cv::waitKey(0);

	return 0;
}
