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
		vision::Point position;
		vision::Point direction;

		Line();
		Line(vision::Point position, vision::Point direction);

		vision::Point lineEnd() const;

		vision::Point lineStart() const;

		float length() const;

		vision::Point perpendicularDir() const;
	};
