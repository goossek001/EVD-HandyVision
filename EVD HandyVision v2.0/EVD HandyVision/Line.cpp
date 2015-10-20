#include "Line.h"
#include "Math.h"

namespace cv{
	Line::Line() : position(), direction() {}
	Line::Line(Point position, Point direction) : position(position), direction(direction) {}

	Point Line::lineEnd() const {
		return position + direction;
	}

	Point Line::lineStart() const {
		return position;
	}

	float Line::length() const {
		return math::length(direction);
	}
}