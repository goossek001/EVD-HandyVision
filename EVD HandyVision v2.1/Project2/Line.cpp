#include "Line.h"
#include "Math.h"

	Line::Line() : position(), direction() {}
	Line::Line(cv::Point position, cv::Point direction) : position(position), direction(direction) {}

	cv::Point Line::lineEnd() const {
		return position + direction;
	}

	cv::Point Line::lineStart() const {
		return position;
	}

	float Line::length() const {
		return math::length(direction);
	}

	cv::Point Line::perpendicularDir() const {
		return cv::Point(direction.y, -direction.x);
	}


	vLine::vLine() : vposition(), vdirection() {}
	vLine::vLine(vision::Point position, vision::Point direction) : vposition(position), vdirection(direction) {}

	vision::Point vLine::vlineEnd() const {
		return vposition + vdirection;
	}

	vision::Point vLine::vlineStart() const {
		return vposition;
	}

	float vLine::vlength() const {
		return math::vlength(vdirection);
	}

	vision::Point vLine::vperpendicularDir() const {
		return vision::Point(vdirection.y, -vdirection.x);
	}