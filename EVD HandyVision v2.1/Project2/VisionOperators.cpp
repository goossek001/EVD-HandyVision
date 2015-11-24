#include "VisionOperators.h"
#include <algorithm>

Point::Point(int x, int y) {
	this->x = x;
	this->y = y;
}

Color::Color(int R, int G, int B, int A) {
	mR = R;
	mG = G;
	mB = B;
	mA = A;
}

int bytesPerPixel(ImageType type) {
	switch (type) {
	default:
		throw "PixelSize(type) is missing the implementation for type '???'";
	case IM_8UC1:
		return 1;
	case IM_8UC3:
		return 3;
	}
}


Mat::Mat(Point size, ImageType type) {
	rows = size.x;
	cols = size.y;
	this->type = type;

	int length = bytesPerPixel(type) * rows * cols;
	data = new unsigned char[length];
	std::fill(data, data + length, 0);
}

Mat::~Mat() {
	delete[] data;
}

Color Mat::get(int i, int j) {
	int pixelSize = bytesPerPixel(type);
	unsigned char* loc = data + pixelSize * j + pixelSize * i * cols;
	Color* color = reinterpret_cast<Color*>(loc);

	return *color;
}
Color Mat::get(Point index) {
	return get(index.y, index.x);
}

void Mat::set(int i, int j, Color color) {
	int pixelSize = bytesPerPixel(type);
	unsigned char* loc = data + pixelSize * j + pixelSize * i * cols;

	*loc = color.R();
	if (pixelSize > 1) {
		++loc;
		*loc = color.G();
		if (pixelSize > 2) {
			++loc;
			*loc = color.B();
			if (pixelSize > 3) {
				++loc;
				*loc = color.A();
			}
		}
	}
}
void Mat::set(Point index, Color color) {
	set(index.y, index.x, color);
}

void bitwise_and(Mat& src1, Mat& src2, Mat& dst) {
	unsigned char* pA = src1.data;
	unsigned char* pB = src2.data;
	unsigned char* pDst = dst.data;

	dst.cols = src1.cols;
	dst.rows = src1.rows;
	dst.type = src1.type;

	int i = src1.rows * src1.cols * bytesPerPixel(src1.type) + 1;
	while (--i) {
		*pDst++ = *pA++ & *pB++;
	}
}

void bitwise_or(const Mat& src1, const Mat& src2, Mat& dst) {
	unsigned char* pA = src1.data;
	unsigned char* pB = src2.data;
	unsigned char* pDst = dst.data;

	dst.cols = src1.cols;
	dst.rows = src1.rows;
	dst.type = src1.type;

	int i = src1.rows * src1.cols * bytesPerPixel(src1.type) + 1;
	while (--i) {
		*pDst++ = *pA++ | *pB++;
	}
}

void bitwise_xor(const Mat& src1, const Mat& src2, Mat& dst) {
	unsigned char* pA = src1.data;
	unsigned char* pB = src2.data;
	unsigned char* pDst = dst.data;

	dst.cols = src1.cols;
	dst.rows = src1.rows;
	dst.type = src1.type;

	int i = src1.rows * src1.cols * bytesPerPixel(src1.type) + 1;
	while (--i) {
		*pDst++ = *pA++ ^ *pB++;
	}
}

void morphologyEx(const Mat& src, Mat& dst, Mor EDOC, Mat& kernel)
{
	unsigned char* pA = src.data;
	unsigned char* pDst = dst.data;
	dst.cols = src.cols;
	dst.rows = src.rows;
	dst.type = src.type;

	switch (EDOC)
	{
	case ERODE:

		int i = src.rows * src.cols * bytesPerPixel(src.type) + 1;
		// start bij kernel /2 + 1 uit de hoek / randen
		
		while (--i) {
			// compare kernel with this spot
			// if kernel fits in scr do nothing.  Else delete the mid pixel from the kernel in dst.
		}
		break;

	case DILATE:
		// #warning create the Dilate function VisionOperators.cpp
		break;

	case OPEN:
		// #warning create the Open function VisionOperators.cpp
		break;

	case CLOSE:
		// #warning create the Close function VisionOperators.cpp
		break;
	}
}