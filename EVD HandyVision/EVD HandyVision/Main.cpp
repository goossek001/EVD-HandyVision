#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "OpenCVExtentions.h"
#include "HandReconisionTools.h"

#include <Windows.h>

int main(int argc, char** argb) {
	cv::Mat src, src_YCrCb, gray, binair, outerEdge, canny, edge;
	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	char* window_name = "Laplace Demo";

	//open image
	src = cv::imread("img2.jpg");
	if (!src.data)
		return -1;

	//HSV skin color filter
	cv::cvtColor(src, src_YCrCb, CV_RGB2YCrCb);
	YCbCrSkinColorFilter(&src_YCrCb, &binair);

	//Detect contour
	DetectAndDrawContour(&binair, &outerEdge, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//Apply canny filter
	cv::cvtColor(src, gray, CV_RGB2GRAY);
	cv::Canny(gray, canny, 120, 255);

	//Combine the outercontour with the canny edge
	cv::bitwise_or(canny, outerEdge, edge);

	//Display the result
	cv::imshow(window_name, edge);

	//Wait until a key is pressed to kill the program
	cv::waitKey(0);

	return 0;
}
