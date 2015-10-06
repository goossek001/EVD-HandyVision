//***************************************************************************************
// Line.h, a class that is used to represent line geometry 
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.01
//***************************************************************************************

#pragma once

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

namespace cv {
	class Line {
	public:
		Point position;
		Point direction;

		Line() : position(), direction() {}
		Line(Point position, Point direction) : position(position), direction(direction) {}

		inline Point lineEnd() const {
			return position + direction;
		}
		inline Point lineStart() const {
			return position;
		}
	};
}