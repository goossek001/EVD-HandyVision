#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

namespace vision {
	struct Point2f;
	struct Point {
		int x, y;

		Point();
		Point(const Point2f& p);
		Point(int x, int y);
		operator cv::Point() { return cv::Point(x, y); }
		Point operator*(const float& i) const;
		Point operator/(const float& i) const;
		Point operator+(const Point& other) const;
		Point operator-(const Point& other) const;
		bool operator==(const Point& other) const;
	};
	struct Point2f {
		float x, y;

		Point2f();
		Point2f(const Point2f& p);
		Point2f(const Point& p);
		Point2f(float x, float y);
		Point2f operator*(const float& i) const;
		Point2f operator/(const float& i) const;
		Point2f operator+(const Point2f& other) const;
		Point2f operator-(const Point2f& other) const;
	};
	struct Point2d {
		double x, y;

		Point2d();
		Point2d(const Point2d& p);
		Point2d(double x, double y);
		Point2d operator*(const double& i) const;
		Point2d operator/(const double& i) const;
		Point2d operator+(const Point2d& other) const;
		Point2d operator-(const Point2d& other) const;
	};
	float dotProd(Point2f v1, Point2f v2);

	struct Color {
		float R, G, B, A;

		Color() {}
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
		IM_32FC1
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
		Mat(const cv::Mat& cvMat);
		operator cv::Mat() const;
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

	void bitwise_and(const Mat& src1, const Mat& src2, Mat& dst);
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
		CLOSE = 3,
		GAUSSIAN = 4
	};

	void morphologyEx(const Mat& src, Mat& dst, Mor EDOC, int kernel);

	void drawRect(const Mat& src, Mat& dst, const RotatadRect& rect, const Color& color = Color(1));

	void distanceTransform(const Mat& src, Mat& dst);

	void bgrtohsv(const Mat& src, Mat& dst);

	void minMaxLoc(const Mat& src, int* min, int* max, Point* minLoc, Point* maxLoc);

	int labelBlobs(const Mat& src, Mat& dst, ConnectionType connected);
	void fillHoles(const Mat& src, Mat& dst, ConnectionType connected);
	void histogram(const Mat& src, unsigned char* hist, int *sum);
	void histgram2D(const Mat& src, Mat& hist, const int channels[2], const int** ranges);

	struct BlobInfo {
		int height;
		int width;
		int nof_pixels;
		float    perimeter;
	};
	struct Rect_aabb {
		Point bottomLeft;
		Point topRight;
	};
	struct Rect_obb {
		Point bottomLeft;
		Point topRight;
		float radians;

		void vertices(Point arr[4]) const;

		inline Point center() const { return (bottomLeft + topRight) / 2; }
		inline int width() const { return abs(bottomLeft.x - topRight.y); }
		inline int height() const { return abs(bottomLeft.y - topRight.y); }
		inline int area() const { return width()*height(); }
	};
	void blobAnalyse(const Mat& img, const int blobcount, BlobInfo* blobInfo);
	bool inBound(const Mat& img, int i, int j);
	bool inBound(const Mat& img, Point point);
	int neighbourCount(const Mat& img, const unsigned char blobnr, int i, int j);
	int neighbourCount(const Mat& img, const unsigned char blobnr, Point p);
	void createCircle(const Mat& src, Mat& dst, const int diameter, const int value, int xCoordinate, int yCoordinate);
	void setSelectedValue(const Mat& src, Mat& dst, int selected, int newVal);
	void split(const Mat& src, Mat channels[3]);
	void HSVThreshold(const Mat& src, Mat& dst,
		double H_min, double H_max,
		double S_min, double S_max,
		double V_min, double V_max);
	void fillHoles(const Mat& src, Mat& dst);
	std::vector<vision::Rect_obb> getBoundingBoxes(const vision::Mat& src);
	std::vector<cv::RotatedRect> cvGetBoundingBoxes(const cv::Mat& src);
	void rotateImage(const cv::Mat& src, cv::Mat& dst, float angle);
	void rotateImage(const vision::Mat& src, vision::Mat& dst, float angle);
	Rect_obb findOMBB(const Mat& img, int blobNr);
	void applyRectMask(const Mat& src, Mat& dst, Rect_obb rect);
	void getBiggestBlob(const Mat& src, Mat& dst);
	void displayOMBB(const Mat& img, int blobNr);
}
