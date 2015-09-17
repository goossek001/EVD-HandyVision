#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "OpenCVExtentions.h"
#include "HandReconisionTools.h"

using namespace cv;

int main(int argc, char** argb) {
	Mat src, src_YCrCb, src_binair;
	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	char* window_name = "Laplace Demo";

	//open image
	src = imread("img.jpg");
	if (!src.data)
		return -1;

	cvtColor(src, src_YCrCb, CV_RGB2YCrCb);
	YCbCrSkinColorFilter(&src_YCrCb, &src_binair);

	//Display the result
	imshow(window_name, src_binair);

	//Wait until a key is pressed to kill the program
	waitKey(0);

	return 0;
}
