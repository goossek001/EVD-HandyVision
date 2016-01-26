//***************************************************************************************
// Functions are specificly used for reconision skin and hands
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.00
//***************************************************************************************

#include "HandReconisionTools.h"
#include "Math.h"

std::map<int, std::string> gestures[GestureType::COUNT];
void initHashTable() {
	//Numbers
	bool fingers[5] = { false };
	gestures[GestureType::DutchCounting][GenerateHashKey(fingers)] = "0";
	fingers[1] = true;
	gestures[GestureType::DutchCounting][GenerateHashKey(fingers)] = "1";
	fingers[2] = true;
	gestures[GestureType::DutchCounting][GenerateHashKey(fingers)] = "2";
	fingers[3] = true;
	gestures[GestureType::DutchCounting][GenerateHashKey(fingers)] = "3";
	fingers[4] = true;
	gestures[GestureType::DutchCounting][GenerateHashKey(fingers)] = "4";
	fingers[0] = true;
	gestures[GestureType::DutchCounting][GenerateHashKey(fingers)] = "5";
	std::fill(fingers, fingers + 5, false);
	fingers[0] = true;
	gestures[GestureType::DutchCounting][GenerateHashKey(fingers)] = "6";
	fingers[1] = true;
	gestures[GestureType::DutchCounting][GenerateHashKey(fingers)] = "7";
	fingers[2] = true;
	gestures[GestureType::DutchCounting][GenerateHashKey(fingers)] = "8";
	fingers[3] = true;
	gestures[GestureType::DutchCounting][GenerateHashKey(fingers)] = "9";

	//Other signs
	fingers[0] = true; fingers[1] = true; fingers[2] = false; fingers[3] = false; fingers[4] = true;
	gestures[GestureType::DutchCounting][GenerateHashKey(fingers)] = "I Love You";
	fingers[0] = false; fingers[1] = false; fingers[2] = true; fingers[3] = false; fingers[4] = false;
	gestures[GestureType::DutchCounting][GenerateHashKey(fingers)] = "!@#$";
}

float length(cv::Point p) {
	return abs(p.x) + abs(p.y);
}

void adaptiveHSVSkinColorFilter(const vision::Mat& src, vision::Mat& dst,
		int H_min, int H_max,
		int S_min, int S_max,
		int V_min, int V_max,
		int S_size, int V_size) {
	vision::Point H = vision::Point(H_min, H_max);
	vision::Point S = vision::Point(S_min, S_max);
	vision::Point V = vision::Point(V_min, V_max);

	vision::Point size = vision::Point(S_size, V_size);
	vision::Point extend = vision::Point(size.x / 2, size.y / 2);
	vision::Point center = vision::Point(S.x + (S.y - S.x) / 2,
		V.x + (V.y - V.x) / 2);

	int sranges[] = { center.x - extend.x, center.x + extend.x };
	int vranges[] = { center.y - extend.y, center.y + extend.y };

	vision::Mat hist;
	int minVal, maxVal;
	vision::Point minLoc, maxLoc;
	for (int i = 0; i < 10; ++i) {

		const int* ranges[] = { sranges, vranges };
		int histSize[] = { size.x, size.x };
		int channels[] = { 1, 2 };
		vision::histgram2D(src, hist, channels, ranges);
		vision::minMaxLoc(hist, &minVal, &maxVal, &minLoc, &maxLoc);

		vision::Point2d avarage;
		int count = 0;
		for (int i = 0; i < hist.rows; ++i) {
			for (int j = 0; j < hist.cols; ++j) {
				if (hist.get(i,j).R >= maxVal * 0.1) {
					avarage = (avarage * count + vision::Point2d(i, j)*hist.get(i, j).R) / (hist.get(i, j).R + count);
					count += hist.get(i, j).R;
				}
			}
		}
		avarage.x += sranges[0];
		avarage.y += vranges[0];

		sranges[0] = avarage.x - extend.x;
		sranges[1] = avarage.x + extend.x;
		vranges[0] = avarage.y - extend.y;
		vranges[1] = avarage.y + extend.y;

		if (sranges[0] < S.x) {
			sranges[0] = S.x;
			sranges[1] = S.x + size.x;
		}
		if (sranges[1] > S.y) {
			sranges[1] = S.y;
			sranges[0] = S.y - size.x;
		}
		if (vranges[0] < V.x) {
			vranges[0] = V.x;
			vranges[1] = V.x + size.y;
		}
		if (vranges[1] > V.y) {
			vranges[1] = V.y;
			vranges[0] = V.y - size.y;
		}

		vision::Point prev = center;
		center.x = sranges[0] + 0.5f * histSize[0];
		center.y = vranges[0] + 0.5f * histSize[1];

		if (math::length(center - prev) < 1)
			break;
	}

	int sRanges[2] = { 256, 0 };
	int vRanges[2] = { 256, 0 };

	for (int i = 0; i < hist.rows; ++i) {
		for (int j = 0; j < hist.cols; ++j) {
			if (sRanges[0] > j + sranges[0]) 
				sRanges[0] = j + sranges[0];
			if (sRanges[1] < j + sranges[0])
				sRanges[1] = j + sranges[0];
			if (vRanges[0] > i + vranges[0])
				vRanges[0] = i + vranges[0];
			if (vRanges[1] < i + vranges[0])
				vRanges[1] = i + vranges[0];
		}
	}
	vision::HSVThreshold(src, dst, H.x, H.y, sRanges[0], sRanges[1], vRanges[0], vRanges[1]);

	vision::getBiggestBlob(dst, dst);
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
/**
Uses a distance trasformation to find the palm center and palm radius
@param src:			binary image of a hand
@param palmCenter:
@param palmRadius:
*/
void getPalmCenter(const vision::Mat& _src, vision::Point& palmCenter, float& palmRadius) {
	//TODO: Uses the opencv distance transform, because our own distance transform has no option for pythagoras
	//Create a distance transform
	cv::Mat dst, dstNormalized;
	cv::Mat src = _src;
	cv::distanceTransform(src, dst, CV_DIST_L2, 3, CV_32FC1);

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
				}
				else {
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
	palmRadius = highestValue;
}

/**
	Look in 4 directions around a pixel and check if all its surrounding pixels are not 0
	@param src:		A binary image of a hand
	@param pos:		The index of the pixel
	@return:		True if all the 4 surrounding pixels are not null
*/
bool isSurrounded(const vision::Mat& src, vision::Point pos) {
	bool isSurrounded = true;
	for (int x = -1; x <= 1; x += 2) {
		for (int y = -1; y <= 1; y += 2) {
			vision::Point neighbour(pos.x + x, pos.y + y);
			if (neighbour.x >= 0 && neighbour.x < src.cols &&
				neighbour.y >= 1 && neighbour.y < src.rows &&
				!src.get(vision::Point(neighbour.x, neighbour.y)).R)
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
void findLargestGap(const vision::Mat& src, vision::Line& out, bool& foundLine, vision::Point center, float maxRadius, float angleStart, float angleEnd) {
	vision::Point previousPoint;
	float previousAngle;
	float largestGap = -1;
	float deltaAngle = PI / 180.0f;
	//Loop to look around all direction around the center
	for (float angle = angleStart; angle < angleEnd; angle += deltaAngle) {
		//Loop through all pixels in a line from center to the circle edge 
		for (int radius = 0; radius < maxRadius; ++radius) {
			vision::Point pixelPos = center + vision::Point(cos(angle) * radius, sin(angle) * radius);
			//Check if the pixel is at the edge of the object
			if (!isSurrounded(src, pixelPos)) {
				if (largestGap == -1) {
					largestGap = 0;
				}
				else if (std::abs(previousAngle - angle) / deltaAngle - 1 > largestGap) {
					//Found a area that has no holes
					largestGap = std::abs(previousAngle - angle) / deltaAngle - 1;
					out.position = previousPoint;
					out.direction = pixelPos - previousPoint;
					foundLine = true;
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
void findWrist(const vision::Mat& src, vision::Line& wristOut, bool& foundWrist, vision::Point palmCenter, float palmRadius) {
	//Look for the rough direction of the wrist by using a large radius. This is done to prefent part of the palm to be miss-inprentended as wrist
	findLargestGap(src, wristOut, foundWrist, palmCenter, 1.75*palmRadius, 0, 3.0f*PI);

	//Look for a better estimation of the wirst position
	float nintyDegrees = 0.5f * PI;
	float minAngle = std::atan2(wristOut.position.y - palmCenter.y, wristOut.position.x - palmCenter.x) - nintyDegrees;
	float maxAngle = std::atan2(wristOut.lineEnd().y - palmCenter.y, wristOut.lineEnd().x - palmCenter.x) + nintyDegrees;
	findLargestGap(src, wristOut, foundWrist, palmCenter, 1.2*palmRadius, minAngle, maxAngle);
}

/**
Create a mask that will cover only the palm of a hand
@param src:			A binary image of a hand
@param dst:			A output image that contains the mask of the palm with same image type as src
@param palmCenter:	The center position of the palm
@param palmRadius:	The radius of the palm in pixels
*/
void createPalmMask(const vision::Mat& src, vision::Mat& dst, vision::Point palmCenter, float palmRadius) {
	//Create a circle that will cover the palm
	dst.create(src.rows, src.cols, src.type);
	createCircle(dst, dst, 1.56f * 2 * palmRadius, 1, palmCenter.x, palmCenter.y);
	vision::bitwise_and(src, dst, dst);
}

/**
	Create a mask that will cover all fingers of the hand
	@param src:				A binary image of a hand
	@param dst:				A output image that contains the mask of the fingers with same image type as src
	@param palmMask:		A mask that covers the palm
	@param wristCenter:		The center position of the wrist
	@param handOrientation:	The direction from wrist to fingers
*/
void createFingerMask(const vision::Mat& src, vision::Mat& dst, vision::Mat& palmMask, vision::Point wristCenter, vision::Point2f handOrientation) {
	//TODO: This function still uses opencv, because findOMBB had just minimal testing

	float size =src.rows + src.cols;
	vision::Point2f bottomLeft = (vision::Point2f)wristCenter + vision::Point2f(-handOrientation.y, handOrientation.x) * 0.5f*size;
	vision::Point2f topRight = (vision::Point2f)wristCenter - vision::Point2f(-handOrientation.y, handOrientation.x) * size + handOrientation * size;
	float rAngle = std::atan2(handOrientation.y, handOrientation.x);

	//Create a finger mask, using the bounding boxes of the fingers
	vision::Rect_obb rRect = vision::Rect_obb{ bottomLeft, topRight, rAngle };
	vision::Point vertices[4];
	rRect.vertices(vertices);
	vision::applyRectMask(src, dst, rRect);

	//Remove the palm
	vision::bitwise_xor(dst, palmMask, dst);
}

/**
	Find the thumb in a list of finger bounding boxes
	@param boundingBoxesFingers:	A list of the bounding boxes of the fingers
	@param palmCenter:				The center position of the palm
	@param handAngle:				The rotation of the hand
	@param thumbDirection:			The direction the thumb points, relative to the hand
	@return:						The index of the thumb in the list boundingBoxesFingers. Return -1 when the is no thumb in the list
*/
int findThumb(const std::vector<cv::RotatedRect>& boundingBoxesFingers, vision::Point palmCenter, float handAngle, ThumbDirection thumbDirection) {
	//Create a rotation matrix
	float rotationMatrix[2][2] = {
		{ cos(handAngle), -sin(handAngle) },
		{ sin(handAngle), cos(handAngle) }
	};
	
	//Loop through all fingers and determen if the finger is a thumb by look at its position the palm 
	bool containsThumb = false;
	for (int i = 0; i < boundingBoxesFingers.size(); ++i) {
		vision::Point2f center(boundingBoxesFingers[i].center.x, boundingBoxesFingers[i].center.y);
		vision::Point direction = center - (vision::Point2f)palmCenter;
		direction = vision::Point(
			direction.x * rotationMatrix[0][0] + direction.y * rotationMatrix[1][0],
			direction.x * rotationMatrix[0][1] + direction.y * rotationMatrix[1][1]
		);
		float rotation = atan2(direction.y, direction.x) / PI * 180;

		if ((rotation >= 0) != (thumbDirection >= 0) && std::abs(rotation) >= 40) {
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
void findPalmLine(const vision::Mat& _srcBinair, vision::Line& _palmLineOut, bool& foundPalm, vision::Line _wristLine, float palmRadius, vision::Point2f _handOrientation, bool isThumbVisible) {
	//TODO: Function still uses opencv, because of a fault in one of the functions (probely rotate point or image)
	cv::Mat srcBinair = _srcBinair;
	cv::Line palmLineOut(cv::Point(_palmLineOut.position.x, _palmLineOut.position.y), cv::Point(_palmLineOut.direction.x, _palmLineOut.direction.y));
	cv::Line wristLine(cv::Point(_wristLine.position.x, _wristLine.position.y), cv::Point(_wristLine.direction.x, _wristLine.direction.y));
	cv::Point2f handOrientation(_handOrientation.x, _handOrientation.y);
	
	//Rotate the image, making the handOrientation (0, -1) 
	Mat srcRotated;
	float angle = atan2(handOrientation.y, handOrientation.x) + 0.5*PI;
	cv::Point temp;
	vision::rotateImage(srcBinair, srcRotated, angle);
	math::rotatePoint(srcBinair, wristLine.position, temp, angle);
	cv::Point temp2;
	math::rotatePoint(srcBinair, wristLine.lineEnd(), temp2, angle);

	//Remove small noise
	Mat kernel = Mat::ones(cv::Point(3, 3), CV_8UC1);
	kernel.at<uchar>(cv::Point(1, 1)) = 0;
	cv::morphologyEx(srcRotated, srcRotated, CV_MOP_CLOSE, kernel);

	int height = temp.y - 0.5f*palmRadius;

	//Look in horizontal lines to find the palm line, by counting the edges
	while (height >= 0 && height < srcRotated.rows) {
		std::vector<cv::Point> intersections = math::horizontalLineObjectIntersection(srcRotated, height);
		for (int i = 1; i < intersections.size(); ++i) {
			if (length(intersections[i - 1] - intersections[i]) < 5){
				intersections.erase(intersections.begin() + i);
				intersections.erase(intersections.begin() + i - 1);
				i = 1;
			}
		}
		int holes = intersections.size() / 2-1;

		int index = -1;
		int largestWidth = -1;

		for (int i = 0; i < holes * 2+1; i += 2) {
			int width = intersections[i+1].x - intersections[i].x;
			if (width > largestWidth) {
				index = i;
				largestWidth = width;
			}
		}
		if (holes >= 1 || largestWidth < 1.66f*palmRadius) {
			if (index != -1) {
				cv::Point v1;
				cv::Point v2;
				if (isThumbVisible) {
					v1 = intersections[index];
					v2 = intersections[index + 1];
				} else {
					v1 = intersections[0];
					v2 = intersections[intersections.size() - 1];
				}

				palmLineOut.position = v1;
				palmLineOut.direction = v2 - v1;

				math::rotateLine(srcRotated, palmLineOut, palmLineOut, -angle);
				foundPalm = true;
			}
			break;
		} 
		--height;
	}

	_palmLineOut.position = vision::Point(palmLineOut.position.x, palmLineOut.position.y);
	_palmLineOut.direction = vision::Point(palmLineOut.direction.x, palmLineOut.direction.y);
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
void labelFingers(std::vector<cv::RotatedRect>& fingersIn, cv::RotatedRect* (&fingersOut)[5], const vision::Point& _wristCenter, const vision::Point& _handOrientation
	, vision::Line _palmLine) {
	const cv::Point wristCenter(_wristCenter.x, _wristCenter.y);
	const cv::Point handOrientation(_handOrientation.x, _handOrientation.y);
	const cv::Line palmLine(cv::Point(_palmLine.position.x, _palmLine.position.y), cv::Point(_palmLine.direction.x, _palmLine.direction.y));
	float palmWidth = math::length(palmLine.direction);

	//Loop through all fingers in 'boundingBoxesFingers' and label them
	for (int i = 0; i < fingersIn.size(); ++i) {
		if (&fingersIn[i] != fingersOut[0]) {
			//Find the edge , in the bounding box, that lays closest to the wrist
			float closestDistance = Infinity;
			cv::Line finger;
			cv::Point2f vertices[4];
			fingersIn[i].points(vertices);
			for (int j = 0; j < 4; ++j) {
				cv::Point v1 = vertices[j];
				cv::Point v2 = vertices[(j + 1) % 4];
				cv::Point center = (cv::Point)(v1 + v2) / 2;
				float distance = math::length(center - wristCenter);
				if (distance < closestDistance) {
					finger.position = center;
					finger.direction = cv::Point(v1.y - v2.y, v2.x - v1.x);
					closestDistance = distance;
				}
			}

			//Determen the finger label by its distance on the palmline
			cv::Point intersect = math::lineLineIntersection(cv::Line(finger.position, -finger.direction), palmLine);
			//cv::line(img, finger.position, finger.position - 2*finger.direction, cv::Scalar(255, 255, 255));
			int index = 1 + math::length(intersect - palmLine.position) / palmWidth * 4;
			if (index > 0 && index < 5) {
				if (!fingersOut[index] || fingersOut[index]->size.area() <= fingersIn[i].size.area())
					fingersOut[index] = &fingersIn[i];
			}
		}
	}
}

void areFingersStretched(cv::RotatedRect* fingers[5], bool(&out)[5], float palmRadius) {
	for (int i = 0; i < 5; ++i) {
		float fingerLength;
		if (fingers[i])
			fingerLength = std::max(fingers[i]->size.width, fingers[i]->size.height);
		else
			fingerLength = 0;
		float multiplier;
		if (i == 0)
			multiplier = 0.24f;
		else
			multiplier = 0.68f;
		out[i] = fingerLength >= multiplier * palmRadius;
	}
}

std::string deteremenGesture(GestureType gestureType, bool fingers[5]) {
	std::map<int, std::string>::iterator it = gestures->find(GenerateHashKey(fingers));
	if (it == gestures->end())
		return "";
	else
		return it->second;
}

int GenerateHashKey(bool fingers[5]) {
	return fingers[0] + 2 * fingers[1] + 4 * fingers[2] + 8 * fingers[3] + 16 * fingers[4];
}