#pragma once

namespace vision {
	struct Point {
		int x, y;

		Point();
		Point(int x, int y);
		Point operator+(const Point& other) const;
		Point operator-(const Point& other) const;
	};
	struct Point2f {
		float x, y;

		Point2f();
		Point2f(float x, float y);
		Point2f operator+(const Point2f& other) const;
		Point2f operator-(const Point2f& other) const;
	};

	struct Color {
		float R, G, B, A;

		Color(float R, float G = 0, float B = 0, float A = 0);
		Color(const Color& other);
	};

	struct RotatadRect {
		Point center;
		Point size;
		float angle;	//In degrees
	};

	enum ImageType {
		IM_8UC1,
		IM_8UC3,
		IM_32SC1
	};

	enum ConnectionType {
		FOUR,
		EIGHT
	};

	int bytesPerPixel(ImageType type);

	struct Mat {
	public:
		unsigned char* data;
		unsigned int rows, cols;
		ImageType type;

		Mat();
		Mat(int rows, int cols, ImageType type);
		Mat(Point size, ImageType type);
		Mat::Mat(const Mat& other);
		~Mat();

		Color get(int i, int j) const;
		Color get(Point index)  const;
		void copyFrom(const Mat& other);

		void create(int rows, int cols, ImageType type);

		void set(int i, int j, Color color);
		void set(Point index, Color color);
	};

	void fill(Mat& img, Color color);

	void bitwise_and(Mat& src1, Mat& src2, Mat& dst);
	void bitwise_or(const Mat& src1, const Mat& src2, Mat& dst);
	void bitwise_xor(const Mat& src1, const Mat& src2, Mat& dst);

	void threshold(const Mat& src, Mat& dst, int lowerbound, int upperbound);
	void split(const Mat& src, Mat dst[]);

	Mat getRotationMatrix2D(Point center, float angle);
	void warpAffine(const Mat& src, Mat& dst, const Mat& R, Point size = Point(-1, -1));
	void warpAffine(const Point2f src, Point2f& dst, const Mat& R);
	void warpAffine(const Point& src, Point& dst, const Mat& R);

	enum Mor
	{
		ERODE = 0,
		DILATE = 1,
		OPEN = 2,
		CLOSE = 3
	};

	void morphologyEx(const Mat& src, Mat& dst, Mor EDOC, Mat& kernel);

	void drawRect(const Mat& src, Mat& dst, const RotatadRect& rect, const Color& color = Color(1));

	void distanceTransform(const Mat& src, Mat& dst);

	void rgbtohsv(const Mat& src, Mat& dst);

	int labelBlobs(const Mat& src, Mat& dst, ConnectionType connected);
	void fillHoles(const Mat& src, Mat& dst, ConnectionType connected);
}
