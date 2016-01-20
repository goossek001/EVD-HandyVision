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
#include "VisionOperators.h"

namespace cv {
	class Line {
	public:
		Point position;
		Point direction;

		Line();
		Line(Point position, Point direction);

		Point lineEnd() const;

		Point lineStart() const;

		float length() const;

		Point perpendicularDir() const;
	};
}

namespace vision {
	class Line {
	public:
		Point position;
		Point direction;

		Line();
		Line(Point position, Point direction);

		Point lineEnd() const;

		Point lineStart() const;

		float length() const;

		Point perpendicularDir() const;
	};
}