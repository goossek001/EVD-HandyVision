#include "HandReconisionTools.h"

#include "OpenCVExtentions.h"

void YCbCrSkinColorFilter(const Mat* src, Mat* dst) {
	cv::cvYCbCrThreshHold(src, dst, 0, 255, 77, 127, 133, 173);
} 