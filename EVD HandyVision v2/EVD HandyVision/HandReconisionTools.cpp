//***************************************************************************************
// Functions are specificly used for reconision skin and hands
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.00
//***************************************************************************************

#include "HandReconisionTools.h"

#include "OpenCVExtentions.h"
#include "Math.h"

/**
	A filter create a binair image that has seperated skin and background
	@param src:		A YCbCr image
	@param dst:		Output as a 8 bit binair image. Skin will have value 1 and non-skin value 0
*/
void YCbCrSkinColorFilter(const Mat& src, Mat& dst) {
	cv::cvYCbCrThreshold(src, dst, 0, 255, 77, 127, 133, 173);
} 

/**
	Sort two blobs on contour area
	@param blob1:
	@param blob2:
*/
bool sortBlobs(std::vector<cv::Point> blob1, std::vector<cv::Point> blob2) {
	return contourArea(blob1) > contourArea(blob2);
}

/**
	Uses a distance trasformation to find the palm center and palm radius
	@param src:			binary image of a hand
	@param palmCenter:
	@param palmRadius:	
*/
void getPalmCenter(const Mat& src, cv::Point& palmCenter, float& palmRadius) {
	//Create a distance transform
	Mat dst, dstNormalized;
	cv::distanceTransform(src, dst, CV_DIST_L2, 3, CV_32F);

	//Loop through all pixels in the distance transform to find the pixels with the highest values (= largestance to edge of the objecct)
	float highestValue = 0;
	int count = 0;
	long xtotal = 0;
	long ytotal = 0;
	for (int x = 0; x < dst.cols; ++x) {
		for (int y = 0; y < dst.rows; ++y) {
			if (dst.at<float>(cv::Point(x, y)) >= highestValue) {
				if (dst.at<float>(cv::Point(x, y)) > highestValue) {
					highestValue = dst.at<float>(cv::Point(x, y));
					count = 1;
					xtotal = x;
					ytotal = y;
				} else {
					count++;
					xtotal += x;
					ytotal += y;
				}
			}
		}
	}

	//Take the avarage position of all high values as the palmcenter
	palmCenter.x = xtotal / count;
	palmCenter.y = ytotal / count;
	palmRadius = dst.at<float>(palmCenter);
}

/**
	Look in 4 directions around a pixel and check if all its surrounding pixels are not 0
	@param src:		A binary image of a hand
	@param pos:		The index of the pixel
	@return:		True if all the 4 surrounding pixels are not null
*/
bool isSurrounded(const Mat& src, cv::Point pos) {
	bool isSurrounded = true;
	for (int x = -1; x <= 1; x += 2) {
		for (int y = -1; y <= 1; y += 2) {
			cv::Point neighbour(pos.x + x, pos.y + y);
			if (neighbour.x >= 0 && neighbour.x < src.cols &&
				neighbour.y >= 1 && neighbour.y < src.rows &&
				!src.at<uchar>(neighbour)) 
			{
				return false;
			}
		}
	}
	return true;
}

/**
	Find the largest pie without holes inside a circle 
	@param src:			A binary image of a hand
	@param out:			The edge of the pie that lays on the edge of the circle
	@param center:		The center of the circle
	@param maxRadius:	The radius of the circle
	@param angleStart:	The start angle of the area that the algoritm will be eximinated by the algoritm
	@param angleEnd:	The end angle of the area that the algoritm will be eximinated by the algoritm
*/
void findLargestGap(const Mat& src, cv::Line& out, cv::Point center, float maxRadius, float angleStart, float angleEnd) {
	cv::Point previousPoint;
	float previousAngle;
	float largestGap = -1;
	float deltaAngle = PI / 180.0f;
	//Loop to look around all direction around the center
	for (float angle = angleStart; angle < angleEnd; angle += deltaAngle) {
		//Loop through all pixels in a line from center to the circle edge 
		for (int radius = 0; radius < maxRadius; ++radius) {
			cv::Point pixelPos = center + cv::Point(cos(angle) * radius, sin(angle) * radius);
			//Check if the pixel is at the edge of the object
			if (!isSurrounded(src, pixelPos)) {
				if (largestGap == -1) {
					largestGap = 0;
				} else if (std::abs(previousAngle - angle) / deltaAngle - 1 > largestGap) {
					//Found a area that has no holes
					largestGap = std::abs(previousAngle - angle) / deltaAngle - 1;
					out.position = previousPoint;
					out.direction = pixelPos - previousPoint;
				}
				previousPoint = pixelPos;
				previousAngle = angle;

				break;	//Continue to look to pixels in a other direction
			}
		}
	}
}

/**
	Find the position of the wrist
	@param src:			A binary image of a hand
	@param wristOut:	A line that is start at one side of the wrist and end at the other side of the wrist
	@param palmCenter:	The center position of the palm
	@param palmRadius:	The radius of the palm in pixels
*/
void findWrist(const Mat& src, cv::Line& wristOut, cv::Point palmCenter, float palmRadius) {
	//Look for the rough direction of the wrist by using a large radius. This is done to prefent part of the palm to be miss-inprentended as wrist
	findLargestGap(src, wristOut, palmCenter, 1.75*palmRadius, 0, 3.0f*PI);

	//Look for a better estimation of the wirst position
	float nintyDegrees = 0.5f * PI;
	float minAngle = std::atan2(wristOut.position.y - palmCenter.y, wristOut.position.x - palmCenter.x) - nintyDegrees;
	float maxAngle = std::atan2(wristOut.lineEnd().y - palmCenter.y, wristOut.lineEnd().x - palmCenter.x) + nintyDegrees;
	findLargestGap(src, wristOut, palmCenter, 1.2*palmRadius, minAngle, maxAngle);
}

/**
	Create a mask that will cover only the palm of a hand
	@param src:			A binary image of a hand
	@param dst:			A output image that contains the mask of the palm with same image type as src
	@param palmCenter:	The center position of the palm
	@param palmRadius:	The radius of the palm in pixels
*/
void createPalmMask(const Mat& src, Mat& dst, cv::Point palmCenter, float palmRadius) {
	//Create a circle that will cover the palm
	cv::Mat circleMask = cv::Mat::zeros(src.rows, src.cols, src.type());
	circle(circleMask, palmCenter, 1.6f * palmRadius, cv::Scalar(255), -1, 8, 0);

	//Create a palm mask, by preforming a binary and on the circlemask and the image of the hand
	dst.create(src.size(), src.type());
	dst.setTo(cv::Scalar(0));
	src.copyTo(dst, circleMask);
}

/**
	Create a mask that will cover all fingers of the hand
	@param src:				A binary image of a hand
	@param dst:				A output image that contains the mask of the fingers with same image type as src
	@param palmMask:		A mask that covers the palm
	@param wristCenter:		The center position of the wrist
	@param handOrientation:	The direction from wrist to fingers
*/
void createFingerMask(const Mat& src, Mat& dst, Mat& palmMask, cv::Point wristCenter, cv::Point2f handOrientation) {
	cv::Point2f rSize(src.rows + src.cols, src.rows + src.cols);
	cv::Point2f rCenter = (cv::Point2f)wristCenter + 0.5f*rSize.x * handOrientation;
	float rAngle = std::atan2(handOrientation.y, handOrientation.x);

	//Create a finger mask, using the bounding boxes of the fingers
	Mat rectMask(src.size(), src.type(), cv::Scalar(0));
	cv::RotatedRect rRect = cv::RotatedRect(rCenter, rSize, rAngle);
	cv::Point2f vertices2f[4];
	rRect.points(vertices2f);
	cv::Point vertices[4];
	for (int i = 0; i < 4; ++i){
		vertices[i] = vertices2f[i];
	}
	cv::fillConvexPoly(rectMask, vertices, 4, cv::Scalar(255));

	//Remove the palm
	cv::bitwise_xor(src, palmMask, dst);
	//Apply the finger mask that was made, using the bounding boxes of the fingers
	cv::bitwise_and(dst, rectMask, dst);
}

/**
	Find the thumb in a list of finger bounding boxes
	@param boundingBoxesFingers:	A list of the bounding boxes of the fingers
	@param palmCenter:				The center position of the palm
	@param handAngle:				The rotation of the hand
	@param thumbDirection:			The direction the thumb points, relative to the hand
	@return:						The index of the thumb in the list boundingBoxesFingers. Return -1 when the is no thumb in the list
*/
int getFindThumb(const std::vector<cv::RotatedRect>& boundingBoxesFingers, cv::Point palmCenter, float handAngle, ThumbDirection thumbDirection) {
	//Create a rotation matrix
	float rotationMatrix[2][2] = {
		{ cos(handAngle), -sin(handAngle) },
		{ sin(handAngle), cos(handAngle) }
	};
	
	//Loop through all fingers and determen if the finger is a thumb by look at its position the palm 
	bool containsThumb = false;
	for (int i = 0; i < boundingBoxesFingers.size(); ++i) {
		cv::Point direction = boundingBoxesFingers[i].center - (cv::Point2f)palmCenter;
		direction = cv::Point(
			direction.x * rotationMatrix[0][0] + direction.y * rotationMatrix[1][0],
			direction.x * rotationMatrix[0][1] + direction.y * rotationMatrix[1][1]
		);
		float rotation = atan2(direction.y, direction.x);
		if (rotation >= 40 * thumbDirection) {
			return i;
		}
	}

	//The list doesm't contains a thumb
	return -1;
}

/**
	Create a line that lays below the 4 fingers, indexfinger till pink
	@param srcBinair:		A binair image of a hand
	@param palmLineOut:		A output variable where the palmline will be saved in (line below the 4 fingers, indexfinger till pink)
	@param handAngle:		The rotation of the hand
	@param thumbDirection:	The direction the thumb is pointing relative to the hand			
*/
void findPalmLine(const Mat& srcBinair, cv::Line& palmLineOut, cv::Line wristLine, cv::Point2f handOrientation, bool isThumbVisible) {
	//Rotate the image, making the handOrientation (0, -1) 
	Mat srcRotated;
	float angle = atan2(handOrientation.y, handOrientation.x) + 0.5*PI;
	cv::Point temp;
	cv::rotateImage(srcBinair, srcRotated, angle);
	math::rotatePoint(srcBinair, wristLine.position, temp, angle);

	int height = temp.y;
	int previousHoleCount = 0;
	int maxHoles = isThumbVisible ? 2 : 1;
	//Look in horizontal lines to find the palm line, by counting the edges
	while (height >= 0) {
		std::vector<cv::Point> intersections = math::horizontalLineObjectIntersection(srcRotated, height);
		int holes = intersections.size() / 2-1;

		if (holes >= maxHoles) {
			height++;
			intersections.clear();
			intersections = math::horizontalLineObjectIntersection(srcRotated, height);
			int index = -1;
			int largestWidth = -1;

			for (int i = 0; i < holes*2; i+=2) {
				int width = intersections[i].x - intersections[i].x;
				if (width > largestWidth) {
					index = i;
					largestWidth = width;
				}
			}

			palmLineOut.position.x = intersections[index].x;
			palmLineOut.position.y = intersections[index].y;
			palmLineOut.direction.x = intersections[index + 1].x - intersections[index].x;
			palmLineOut.direction.y = intersections[index + 1].y - intersections[index].y;
			math::rotateLine(srcBinair, palmLineOut, palmLineOut, angle);
			break;
		} else if (previousHoleCount > holes){
			maxHoles = 1;
		}
		--height;
		previousHoleCount = holes;
	}
}

/**
	Label the 4 fingers, indexfinger till pink
	@param boundingBoxesFingers:			A list with bounding boxes of each finger
	@param wristCenter:						The center position of the wrist
	@param handOrientation:					The direction from wrist to fingers
	@param palmLine:						A line ust below the 4 fingers, indexfinger till pink
	@param thumbIndex:						The index of the thumb in the list boundingBoxesFingers
	@param thumbIndexindexFingerIndexOut:	The index of the index finger in the list boundingBoxesFingers
	@param middleFingerIndexOut:			The index of the middle finger in the list boundingBoxesFingers
	@param ringFingerIndexOut:				The index of the ring finger in the list boundingBoxesFingers
	@param pinkyIndexOut:					The index of the pink in the list boundingBoxesFingers
*/
void labelFingers(const std::vector<cv::RotatedRect>& boundingBoxesFingers, const cv::Point& wristCenter, const cv::Point& handOrientation
	, cv::Line palmLine, int thumbIndex, int& indexFingerIndexOut , int& middleFingerIndexOut, int& ringFingerIndexOut, int& pinkyIndexOut) {
	indexFingerIndexOut = -1;
	middleFingerIndexOut = -1;
	ringFingerIndexOut = -1;
	pinkyIndexOut = -1;

	float palmWidth = math::length(palmLine.direction);
	//Loop through all fingers in 'boundingBoxesFingers' and label them
	for (int i = 0; i < boundingBoxesFingers.size(); ++i) {
		if (i != thumbIndex) {
			//Find the edge , in the bounding box, that lays closest to the wrist
			float closestDistance = Infinity;
			cv::Point fingerPosition;
			cv::Point2f vertices[4];
			boundingBoxesFingers[i].points(vertices);
			for (int j = 0; j <= 4; ++j) {
				cv::Point point = (cv::Point)(vertices[j] + vertices[(j + 1) % 4]) / 2;
				float distance = math::length(point - wristCenter);
				if (distance < closestDistance) {
					fingerPosition = point;
					closestDistance = distance;
				}
			}

			//Determen the finger label by its distance on the palmline
			cv::Point intersect = math::lineLineIntersection(cv::Line(fingerPosition, -handOrientation), palmLine);
			int finger = 1 + math::length(intersect - palmLine.position) / palmWidth * 4;
			if (finger <= 1)
				indexFingerIndexOut = i;
			else if (finger == 2)
				middleFingerIndexOut = i;
			else if (finger == 3)
				ringFingerIndexOut = i;
			else
				pinkyIndexOut = i;
		}
	}
}