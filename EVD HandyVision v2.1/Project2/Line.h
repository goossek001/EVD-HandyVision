//***************************************************************************************
// Line.h, a class that is used to represent line geometry 
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.01
//***************************************************************************************

#pragma once

#include "VisionOperators.h"


	class Line {
	public:
		cv::Point position;
		cv::Point direction;

		Line();
		Line(cv::Point position, cv::Point direction);

		cv::Point lineEnd() const;

		cv::Point lineStart() const;

		float length() const;

		cv::Point perpendicularDir() const;
	};

	class vLine {
	public:
		vision::Point vposition;
		vision::Point vdirection;

		vLine();
		vLine(vision::Point vposition, vision::Point vdirection);

		vision::Point vlineEnd() const;

		vision::Point vlineStart() const;

		float vlength() const;

		vision::Point vperpendicularDir() const;
	};
