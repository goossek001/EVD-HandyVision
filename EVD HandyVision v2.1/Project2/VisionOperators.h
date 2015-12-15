#pragma once

struct Point {
	int x, y;

	Point(int x, int y);
};

struct Color {
	unsigned char mR, mG, mB, mA;

	Color(int R, int G = 0, int B = 0, int A = 0);

	inline int R() { return (int)mR; }
	inline int G() { return (int)mG; }
	inline int B() { return (int)mB; }
	inline int A() { return (int)mA; }
};

enum ImageType {
	IM_8UC1,
	IM_8UC3
};


int bytesPerPixel(ImageType type);

struct Mat {
public:
	unsigned char* data;
	unsigned int rows, cols;
	ImageType type;

	Mat(Point size, ImageType type);
	~Mat();

	Color get(int i, int j) const;
	Color get(Point index)  const;

	void set(int i, int j, Color color);
	void set(Point index, Color color);
};

void bitwise_and(Mat& src1, Mat& src2, Mat& dst);
void bitwise_or(const Mat& src1, const Mat& src2, Mat& dst);
void bitwise_xor(const Mat& src1, const Mat& src2, Mat& dst);

enum Mor
{
	ERODE = 0,
	DILATE = 1,
	OPEN = 2,
	CLOSE = 3
};
void morphologyEx(const Mat& src, Mat& dst, Mor EDOC, int kernel);