//***************************************************************************************
// Line.h, a class that is used to represent line geometry 
// Autors:	Kay Goossen
// Date:	28 September 2015
// Version: 1.0
//***************************************************************************************

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

namespace cv {
	class Line {
	public:
		Point position;
		Point direction;

		Line(Point position, Point direction) : position(position), direction(direction) {}
	};
}