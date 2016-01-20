//***************************************************************************************
// Functions that manipulate images
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.00
//***************************************************************************************

#include "OpenCVExtentions.h"
#include "Math.h"
#include <limits>
#include "VisionOperators.h"

namespace vision {
	void HSVThreshold(const Mat& src, Mat& dst,
		double H_min, double H_max,
		double S_min, double S_max,
		double V_min, double V_max) {
		//Split into channels
		Mat channels[3];
		split(src, channels);

		//Apply thresholds
		threshold(channels[0], channels[0], H_min, H_max);
		threshold(channels[1], channels[1], S_min, S_max);
		threshold(channels[2], channels[2], V_min, V_max);

		fill(channels[1], Color(1));

		//Combine threshold images
		bitwise_and(channels[0], channels[1], dst);
		bitwise_and(dst, channels[2], dst);
	}

	///**
	//Find a rotated bounding boxes for each blob
	//@param src: a 8 bit binair image
	//*/
	//std::vector<Rect_obb> getBoundingBoxes(const Mat& src) {
	//	std::vector<Rect_obb> boundingRects;
	//	Mat temp;
	//	int nrOfBlobs = labelBlobs(src, temp, FOUR);
	//	for (int i = 1; i <= nrOfBlobs; ++i) {
	//		boundingRects.push_back(findOMBB(src, i));
	//	}
	//	return boundingRects;
	//}

	/**
	Find a rotated bounding boxes for each blob
	@param src: a 8 bit binair image
	*/
	std::vector<cv::RotatedRect> getBoundingBoxes(const cv::Mat& src) {
		int minAreaThreshold = 16;
		std::vector<cv::Vec4i> hierarchy;
		std::vector<std::vector<cv::Point> > contours;

		cv::Mat srcCopy;
		src.copyTo(srcCopy);

		std::vector<cv::RotatedRect> boundingBoxes;
		cv::findContours(srcCopy, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		for (int i = 0; i < contours.size(); ++i)
		if (contourArea(contours[i]) >= minAreaThreshold)
			boundingBoxes.push_back(cv::minAreaRect(contours[i]));

		return boundingBoxes;
	}

	/**
	Rotate a image around its center
	@param src:			A image with no type restrictions
	@param dst			The output channel for the rotated image. This image will have the same type and parent as the param 'src'
	@param angle:		The rotation of the image in radians
	*/
	void rotateImage(const cv::Mat& src, cv::Mat& dst, float angle) {
		int len = std::max(src.cols, src.rows);
		cv::Point2f pt(len / 2., len / 2.);
		cv::Mat r = cv::getRotationMatrix2D(pt, angle / PI * 180, 1.0);

		cv::warpAffine(src, dst, r, cv::Size(len, len));
	}void rotateImage(const vision::Mat& src, vision::Mat& dst, float angle) {
		int len = std::max(src.cols, src.rows);
		vision::Point2f pt(len / 2., len / 2.);
		vision::Mat r = vision::getRotationMatrix2D(pt, angle / PI * 180);

		vision::warpAffine(src, dst, r, vision::Point(len, len));
	}

	/**
	Only keep the pixels of a image that lay inside a bounding rect
	@param src:			A image with no type restrictions
	@param dst			The src image after applying the mask
	@param boundingRect: The shape of the mask
	*/
	void applyRectangleMask(const Mat& src, Mat& dst, Rect_obb boundingRect) {
		applyRectMask(src, dst, boundingRect);
	}

	/**
	Only keep the biggest contour suposeingly its the hand
	@param src:			An binairy image
	@param dst			The binairy src image after applying the mask and give only the biggest contour.
	*/
	void getBiggestBlob(const Mat& src, Mat& dst){
		BlobInfo* blobInfo;
		Mat temp;
		int nrOfBlobs = labelBlobs(src, temp, FOUR);
		blobInfo = new BlobInfo[nrOfBlobs];
		blobAnalyse(temp, nrOfBlobs, blobInfo);
		int biggestBlob_id = 1;
		int biggestBlob_perimeter = 0;
		for (int i = 0; i < nrOfBlobs; ++i) {
			if (blobInfo[i].perimeter > biggestBlob_perimeter) {
				biggestBlob_perimeter = blobInfo[i].perimeter;
				biggestBlob_id = i+1;
			}
		}
		threshold(src, dst, biggestBlob_id, biggestBlob_id);

		delete[] blobInfo;
	}
}

