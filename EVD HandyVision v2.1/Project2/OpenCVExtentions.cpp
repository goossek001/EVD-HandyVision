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

namespace cv {
	void threshold(const Mat& src, Mat& dst, int lowerbound, int upperbound) {
			if (lowerbound <= upperbound) {
				inRange(src, lowerbound, upperbound, dst);
			}
			else {
				Mat temp;
				inRange(src, lowerbound, 255, temp);
				inRange(src, 0, upperbound, dst);
				bitwise_or(dst, temp, dst);
			}
	}

	void HSVThreshold(const Mat& src, Mat& dst,
		double H_min, double H_max,
		double S_min, double S_max,
		double V_min, double V_max) {
		//Split into channels
		cv::Mat channels[3];
		cv::split(src, channels);

		//Apply thresholds
		cv::threshold(channels[0], channels[0], H_min, H_max);
		cv::threshold(channels[1], channels[1], S_min, S_max);
		cv::threshold(channels[2], channels[2], V_min, V_max);

		//Combine threshold images
		cv::bitwise_and(channels[0], channels[1], dst);
		cv::bitwise_and(dst, channels[2], dst);
	}

	/**
	Detect and the contour of objects
	@param src: a 8 bit binair image
	@param dst: output as a 8 bit binair image
	*/
	void detectAndDrawContour(const Mat& src, Mat& dst, int mode, int method, int lineWidth, int minAreaThreshold) {
		Mat srcCopy;
		src.copyTo(srcCopy);

		//Detect contours
		std::vector<cv::Vec4i> hierarchy;
		std::vector<std::vector<cv::Point> > contours;
		cv::findContours(srcCopy, contours, hierarchy, mode, method);

		//Draw contours
		dst.create(src.size(), CV_8UC1);
		dst.setTo(cv::Scalar(0));
		for (size_t i = 0; i<contours.size(); ++i)
		if (contourArea(contours[i]) >= minAreaThreshold)
			drawContours(dst, contours, i, cv::Scalar(255), lineWidth);
	}

	/**
	Fill the holes inside the blobs
	@param src: a 8 bit binair image
	@param dst: output as a 8 bit binair image without holes
	*/
	void fillHoles(const Mat& src, Mat& dst) {
		detectAndDrawContour(src, dst, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, -1);
	}

	/**
	Find a rotated bounding boxes for each blob
	@param src: a 8 bit binair image
	*/
	std::vector<cv::RotatedRect> getBoundingBoxes(const Mat& src) {
		int minAreaThreshold = 16;
		std::vector<cv::Vec4i> hierarchy;
		std::vector<std::vector<cv::Point> > contours;

		Mat srcCopy;
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
	}

	/**
	Only keep the pixels of a image that lay inside a bounding rect
	@param src:			A image with no type restrictions
	@param dst			The src image after applying the mask
	@param boundingRect: The shape of the mask
	*/
	void applyRectangleMask(const cv::Mat& src, cv::Mat& dst, RotatedRect boundingRect) {
		Mat rectMask = Mat::zeros(src.size(), CV_8UC1);
		Point2f vertices2f[4];
		Point vertices[4];
		boundingRect.points(vertices2f);
		for (int i = 0; i < 4; i++) {
			vertices[i] = vertices2f[i];
			line(rectMask, vertices2f[i], vertices2f[(i + 1) % 4], Scalar(255));
		}
		cv::fillConvexPoly(rectMask, vertices, 4, cv::Scalar(255));

		src.copyTo(dst, rectMask);
	}

	/**
	Only keep the biggest contour suposeingly its the hand
	@param src:			An binairy image
	@param dst			The binairy src image after applying the mask and give only the biggest contour.
	*/
	void getContour(cv::Mat& src, cv::Mat& dst){

		int largest_area = 0;
		int largest_contour_index = 0;

		cv::Mat contourOutput(src.rows, src.cols, CV_8UC1, Scalar::all(0));
		cv::Mat contourImage = src.clone();

		std::vector<std::vector<cv::Point> > contours;
		std::vector<Vec4i> hierarchy;
		cv::findContours(contourImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); // Find the contours in the image

		for (int i = 0; i< contours.size(); i++) // iterate through each contour. 
		{
			double a = contourArea(contours[i], false);  //  Find the area of contour
			if (a>largest_area){
				largest_area = a;
				largest_contour_index = i;                //Store the index of largest contour
			}

		}

		Scalar color(255, 255, 255);
		cv::drawContours(contourOutput, contours, largest_contour_index, color, CV_FILLED, 8, hierarchy); // Draw the largest contour using previously stored index.
		contourOutput.copyTo(dst);
	}
}