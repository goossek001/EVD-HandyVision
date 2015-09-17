#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "OpenCVExtentions.h"
#include "HandReconisionTools.h"

#include <Windows.h>

int main(int argc, char** argb) {
	cv::Mat src, src_YCrCb, binair, edge;
	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	char* window_name = "Laplace Demo";

	//open image
	src = cv::imread("img.jpg");
	if (!src.data)
		return -1;

	cv::cvtColor(src, src_YCrCb, CV_RGB2YCrCb);
	YCbCrSkinColorFilter(&src_YCrCb, &binair);

	DetectAndDrawContour(&binair, &edge, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//Display the result
	cv::imshow(window_name, edge);

	//Wait until a key is pressed to kill the program
	cv::waitKey(0);

	return 0;
}
