//***************************************************************************************
// Functions are specificly used for reconision skin and hands
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.00
//***************************************************************************************

#include "HandReconisionTools.h"
#include "Math.h"

<<<<<<< HEAD
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


void biggestColorBlob(const Mat& src, Mat& dst, const Mat& mask) {
	throw "not implemented error";
	/*
	//Create histogram
	float fullRange[2] = { 0, 255 };
	const float* ranges[] = { fullRange, fullRange, fullRange };
	int histSize[] = { 255, 255, 255 };
	int channels[] = { 0, 1, 2 };
	cv::MatND hist;
	calcHist(&src, 1, channels, Mat(), // do not use mask
=======

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

	float length(vision::Point p) {
		return abs(p.x) + abs(p.y);
	}


	void biggestColorBlob(const vision::Mat& src, vision::Mat& dst, const vision::Mat& mask) {
		throw("No biggestColorBlob implemented");
		/*
		//Create histogram
		float fullRange[2] = { 0, 255 };
		const float* ranges[] = { fullRange, fullRange, fullRange };
		int histSize[] = { 255, 255, 255 };
		int channels[] = { 0, 1, 2 };
		vision::Mat hist;
		calcHist(&src, 1, channels, Mat(), // do not use mask
>>>>>>> does NOT WORK, maar wel handrecognision tools functies gedaan
		hist, 3, histSize, ranges,
		true, // the histogram is uniform
		false);

		//Label blobs

<<<<<<< HEAD
	//TODO: Label blobs
	//TODO: Caclulate distance between blobs (center to center)
	//TODO: Merge blobs until two blobs are remaining, giving closer blobs prioritiy
	//TODO: Determen wich blob is skin color, by analyising the blobs and determen wich shape is more likely to be a hand or a head
	*/
}
=======
		//TODO: Label blobs
		//TODO: Caclulate distance between blobs (center to center)
		//TODO: Merge blobs until two blobs are remaining, giving closer blobs prioritiy
		//TODO: Determen wich blob is skin color, by analyising the blobs and determen wich shape is more likely to be a hand or a head
		*/
	}
>>>>>>> does NOT WORK, maar wel handrecognision tools functies gedaan

	void adaptiveHSVSkinColorFilter(const Mat& src, Mat& dst,
		int H_min, int H_max,
		int S_min, int S_max,
		int V_min, int V_max,
		int S_size, int V_size) {
		/*vision::Point H = vision::Point(0.9 * 255, 0.2 * 255);
		vision::Point S = vision::Point(0.35 * 255, 0.95*255);
		vision::Point V = vision::Point(0.15 * 255, 0.75 * 255);*/

		vision::Point H = vision::Point(H_min, H_max);
		vision::Point S = vision::Point(S_min, S_max);
		vision::Point V = vision::Point(V_min, V_max);

		vision::Point size = vision::Point(S_size, V_size);
		vision::Point extend = vision::Point(size.x / 2, size.y / 2);
		vision::Point center = vision::Point(S.x + (S.y - S.x) / 2,
			V.x + (V.y - V.x) / 2);

		float sranges[] = { center.x - extend.x, center.x + extend.x };
		float vranges[] = { center.y - extend.y, center.y + extend.y };

		unsigned char hist[255];
		double maxVal;
		vision::Point maxLoc;
		for (int i = 0; i < 10; ++i) {

			const float* ranges[] = { sranges, vranges };
			int histSize[] = { size.x, size.x };
			int channels[] = { 1, 2 };
			vision::histogram(&src, hist);
			vision::minMaxLoc(hist, 0, &maxVal, 0, &maxLoc);  // todo..

			cv::Point2d avarage;
			int count = 0;
			for (int i = 0; i < hist.rows; ++i) {
				for (int j = 0; j < hist.cols; ++j) {
					if (hist.at<float>(i, j) >= maxVal * 0.1) {
						avarage = (avarage * count + cv::Point2d(i, j)*hist.at<float>(i, j)) / (hist.at<float>(i, j) + count);
						count += hist.at<float>(i, j);
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
			center.x = sranges[1] - sranges[0];
			center.y = vranges[1] - vranges[0];

			if (length(center - prev) < 1)
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

		cv::HSVThreshold(src, dst, H.x, H.y, sRanges[0], sRanges[1], vRanges[0], vRanges[1]);

		//biggestColorBlob(src, dst, dst);

		Mat temp;
		cv::HSVThreshold(src, temp, H.x, H.y, S.x, S.y, V.x, V.y);
		cv::imshow("fullRange", temp);
		cv::imshow("binair", dst);
	}

	void CannyHandFilter(const vision::Mat& src, vision::Mat& dst) {
		throw ("Error no Canny implemented");
		/*	Mat channels[3];
			split(src, channels);

			Mat copy;

			cv::GaussianBlur(src, copy, cv::Size(11, 11), 0, 0);

			cv::Canny(copy, dst, 40, 60);
			Mat kernel = Mat::ones(cv::Point(5, 5), CV_8UC1);
			cv::morphologyEx(dst, dst, CV_MOP_DILATE, kernel);
			cv::line(dst, cv::Point(1, 1), cv::Point(dst.cols - 2, 1), cv::Scalar(255));
			cv::fillHoles(dst, dst);
			cv::morphologyEx(dst, dst, CV_MOP_ERODE, kernel);*/
	}

	/**
		Sort two blobs on contour area
		@param blob1:
		@param blob2:
		*/
	bool sortBlobs(std::vector<vision::Point> blob1, std::vector<vision::Point> blob2) {
		throw ("Error no sortBlobs implemented, use blobAnalyse");
		/*return contourArea(blob1) > contourArea(blob2);*/
	}
<<<<<<< HEAD
	vision::Mat vSrc = vision::Mat(src);
	vision::Mat vDst;
	vision::HSVThreshold(vSrc, vDst, H.x, H.y, sRanges[0], sRanges[1], vRanges[0], vRanges[1]);
	dst = vDst;

	//biggestColorBlob(src, dst, dst);
}

void CannyHandFilter(const Mat& src, Mat& dst) {
	throw "Not implemented error";
	/*Mat channels[3];
	split(src, channels);

	Mat copy;

	cv::GaussianBlur(src, copy, cv::Size(11, 11), 0, 0);

	cv::Canny(copy, dst, 40, 60);
	Mat kernel = Mat::ones(cv::Point(5, 5), CV_8UC1);
	cv::morphologyEx(dst, dst, CV_MOP_DILATE, kernel);
	cv::line(dst, cv::Point(1, 1), cv::Point(dst.cols - 2, 1), cv::Scalar(255));
	cv::fillHoles(dst, dst);
	cv::morphologyEx(dst, dst, CV_MOP_ERODE, kernel);*/
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
=======

	/**
		Uses a distance trasformation to find the palm center and palm radius
		@param src:			binary image of a hand
		@param palmCenter:
		@param palmRadius:
		*/
	void getPalmCenter(const vision::Mat& src, vision::Point& palmCenter, float& palmRadius) {
		//Create a distance transform
		vision::Mat dst, dstNormalized;
		vision::Mat visionSrc = vision::Mat (src);
		vision::distanceTransform(visionSrc, dst);

		//Loop through all pixels in the distance transform to find the pixels with the highest values (= largestance to edge of the objecct)
		float highestValue = 0;
		int count = 0;
		long xtotal = 0;
		long ytotal = 0;
		for (int x = 0; x < dst.cols; ++x) {
			for (int y = 0; y < dst.rows; ++y) {
				if (dst.get(vision::Point(x, y)).R >= highestValue) {
					if (dst.get(vision::Point(x, y)).R > highestValue) {
						highestValue = (dst.get(vision::Point(x, y)).R);
						count = 1;
						xtotal = x;
						ytotal = y;
					}
					else {
						count++;
						xtotal += x;
						ytotal += y;
					}
>>>>>>> does NOT WORK, maar wel handrecognision tools functies gedaan
				}
			}
		}

		//Take the avarage position of all high values as the palmcenter
		palmCenter.x = xtotal / count;
		palmCenter.y = ytotal / count;
		palmRadius = dst.get(vision::Point(palmCenter.x, palmCenter.y)).R;
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
	void findLargestGap(const vision::Mat& src, Line& out, bool& foundLine, vision::Point center, float maxRadius, float angleStart, float angleEnd) {
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
	void findWrist(const vision::Mat& src, Line& wristOut, bool& foundWrist, vision::Point palmCenter, float palmRadius) {
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
		vision::Point2f rSize(src.rows + src.cols, src.rows + src.cols);
		vision::Point2f rCenter = (vision::Point2f)wristCenter + 0.5f*rSize.x * handOrientation;
		float rAngle = std::atan2(handOrientation.y, handOrientation.x) * 180 / PI;

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
		vision::bitwise_xor(src, palmMask, dst);
		//Apply the finger mask that was made, using the bounding boxes of the fingers
		vision::bitwise_and(dst, rectMask, dst);
	}

<<<<<<< HEAD
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
void findPalmLine(const Mat& srcBinair, cv::Line& palmLineOut, bool& foundPalm, cv::Line wristLine, float palmRadius, cv::Point2f handOrientation, bool isThumbVisible) {
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
			if (length(intersections[i - 1] - intersections[i]) < 3){
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
=======
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
			float rotation = atan2(direction.y, direction.x) / PI * 180;

			if ((rotation >= 0) != (thumbDirection >= 0) && std::abs(rotation) >= 40) {
				return i;
			}
		}
>>>>>>> does NOT WORK, maar wel handrecognision tools functies gedaan

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
	void findPalmLine(const Mat& srcBinair, cv::Line& palmLineOut, bool& foundPalm, cv::Line wristLine, float palmRadius, cv::Point2f handOrientation, bool isThumbVisible) {
		//Rotate the image, making the handOrientation (0, -1) 
		Mat srcRotated;
		float angle = atan2(handOrientation.y, handOrientation.x) + 0.5*PI;
		cv::Point temp;
		cv::rotateImage(srcBinair, srcRotated, angle);
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
			int holes = intersections.size() / 2 - 1;

			int index = -1;
			int largestWidth = -1;

			for (int i = 0; i < holes * 2 + 1; i += 2) {
				int width = intersections[i + 1].x - intersections[i].x;
				if (width > largestWidth) {
					index = i;
					largestWidth = width;
				}
			}
			if (holes >= 1 || largestWidth < 1.75f*palmRadius) {
				if (index != -1) {
					cv::Point v1;
					cv::Point v2;
					if (isThumbVisible) {
						v1 = intersections[index];
						v2 = intersections[index + 1];
					}
					else {
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
	void labelFingers(std::vector<cv::RotatedRect>& fingersIn, cv::RotatedRect* (&fingersOut)[5], const cv::Point& wristCenter, const cv::Point& handOrientation
		, cv::Line palmLine) {
		float palmWidth = math::length(palmLine.direction);

		//Mat img = Mat::zeros(cv::Size(640, 480), CV_8UC3);
		//cv::line(img, palmLine.position + 0.00f*palmLine.direction, palmLine.position + 0.25f*palmLine.direction, cv::Scalar(255, 0, 0));
		//cv::line(img, palmLine.position + 0.25f*palmLine.direction, palmLine.position + 0.50f*palmLine.direction, cv::Scalar(0, 0, 255));
		//cv::line(img, palmLine.position + 0.50f*palmLine.direction, palmLine.position + 0.75f*palmLine.direction, cv::Scalar(0, 255, 0));
		//cv::line(img, palmLine.position + 0.75f*palmLine.direction, palmLine.position + 1.00f*palmLine.direction, cv::Scalar(255, 0, 0));

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
		//imshow("fingers", img);
	}
<<<<<<< HEAD
	//imshow("fingers", img);
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

void displayFingers(const Mat& img, cv::RotatedRect* fingers[5]) {
	throw "Not implemented error";
	/*
	//Create a seperate image of each finger
	Mat thumb, indexFinger, middleFinger, ringFinger, pinky;
	Mat empty = Mat::zeros(img.size(), img.type());
	if (fingers[0]) {
		cv::applyRectangleMask(img, thumb, *fingers[0]);
		imshow("thumb", thumb);
	} else {
		imshow("thumb", empty);
	}
	if (fingers[1]){
		cv::applyRectangleMask(img, indexFinger, *fingers[1]);
		imshow("indexFinger", indexFinger);
	} else {
		imshow("indexFinger", empty);
=======

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
				multiplier = 0.5f;
			out[i] = fingerLength >= multiplier * palmRadius;
		}
>>>>>>> does NOT WORK, maar wel handrecognision tools functies gedaan
	}

	void displayFingers(const Mat& img, cv::RotatedRect* fingers[5]) {
		//Create a seperate image of each finger
		Mat thumb, indexFinger, middleFinger, ringFinger, pinky;
		Mat empty = Mat::zeros(img.size(), img.type());
		if (fingers[0]) {
			cv::applyRectangleMask(img, thumb, *fingers[0]);
			imshow("thumb", thumb);
		}
		else {
			imshow("thumb", empty);
		}
		if (fingers[1]){
			cv::applyRectangleMask(img, indexFinger, *fingers[1]);
			imshow("indexFinger", indexFinger);
		}
		else {
			imshow("indexFinger", empty);
		}
		if (fingers[2]){
			cv::applyRectangleMask(img, middleFinger, *fingers[2]);
			imshow("middleFinger", middleFinger);
		}
		else {
			imshow("middleFinger", empty);
		}
		if (fingers[3]){
			cv::applyRectangleMask(img, ringFinger, *fingers[3]);
			imshow("ringFinger", ringFinger);
		}
		else {
			imshow("ringFinger", empty);
		}
		if (fingers[4]) {
			cv::applyRectangleMask(img, pinky, *fingers[4]);
			imshow("pinky", pinky);
		}
		else {
			imshow("pinky", empty);
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
<<<<<<< HEAD
	*/
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
=======
>>>>>>> does NOT WORK, maar wel handrecognision tools functies gedaan
