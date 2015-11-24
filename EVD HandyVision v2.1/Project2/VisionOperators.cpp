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

void bitwise_and(const Mat& src1, const Mat& src2, const Mat& dst) {

	for (int i = 0; i < src1.rows; i++){
		for (int j = 0; j < src1.cols; i++){
			dst[i][j] = src1 & 1 << src2;
		}
	}

	return;
}


void bitwise_or(const Mat& src1, const Mat& src2, const Mat& dst){
	//	|
	return;
}
void bitwise_xor(const Mat& src1, const Mat& src2, const Mat& dst){
	//	^
	return;
	}