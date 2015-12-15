#pragma once

namespace vision {
	struct Point {
		int x, y;

		Point(int x, int y);
	};

	struct Color {
		float R, G, B, A;

		Color(float R, float G = 0, float B = 0, float A = 0);
		Color(const Color& other);
	};

	enum ImageType {
		IM_8UC1,
		IM_8UC3,
		IM_32SC1
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

		void copyFrom(const Mat& other);

		void create(int rows, int cols, ImageType type);

		Color get(int i, int j) const;
		Color get(Point index) const;

		void set(int i, int j, Color color);
		void set(Point index, Color color);
	};

	void bitwise_and(const Mat& src1, const Mat& src2, Mat& dst);
	void threshold(const Mat& src, Mat& dst, int lowerbound, int upperbound);
	void split(const Mat& src, Mat dst[]);

	Mat getRotationMatrix2D(Point center, float angle);
	void warpAffine(const Mat& src, Mat& dst, const Mat& R, Point size = Point(-1, -1));
}