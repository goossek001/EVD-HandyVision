#include "VisionOperators.h"
#include <algorithm>

namespace vision {
#define PI 3.14159265359f
#define TWO_PI 6.28318530718f

	Point::Point() {
		x = 0;
		y = 0;
	}
	Point::Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
	Point Point::operator+(const Point& other) const {
		return Point(x + other.x, y + other.y);
	}
	Point Point::operator-(const Point& other) const {
		return Point(x - other.x, y - other.y);
	}
	Point2f::Point2f() {
		x = 0;
		y = 0;
	}
	Point2f::Point2f(float x, float y) {
		this->x = x;
		this->y = y;
	}
	Point2f Point2f::operator+(const Point2f& other) const {
		return Point2f(x + other.x, y + other.y);
	}
	Point2f Point2f::operator-(const Point2f& other) const {
		return Point2f(x - other.x, y - other.y);
	}

	Color::Color(float R, float G, float B, float A) {
		this->R = R;
		this->G = G;
		this->B = B;
		this->A = A;
	}

	Color::Color(const Color& other) : Color(other.R, other.G, other.B, other.A) {	}

	int bytesPerPixel(ImageType type) {
		switch (type) {
		default:
			throw "PixelSize(type) is missing the implementation for type '???'";
		case IM_8UC1:
			return 1;
		case IM_8UC3:
			return 3;
		case IM_32SC1:
			return 4;
		}
	}

	Mat::Mat() : Mat(0, 0, IM_8UC1) {}

	Mat::Mat(const Mat& other) {
		this->rows = other.rows;
		this->cols = other.cols;
		this->type = other.type;

		int length = bytesPerPixel(type) * rows * cols;
		data = new unsigned char[length];

		unsigned char* src = other.data;
		unsigned char* dst = this->data;

		++length;
		while (--length) {
			*(dst++) = *(src++);
		}
	}

	Mat::Mat(int rows, int cols, ImageType type) {
		this->rows = rows;
		this->cols = cols;
		this->type = type;

		int length = bytesPerPixel(type) * rows * cols;
		data = new unsigned char[length];
		std::fill(data, data + length, 0);
	}


	Mat::Mat(Point size, ImageType type) : Mat(size.y, size.x, type) { }

	Mat::~Mat() {
		delete[] data;
	}

	void Mat::copyFrom(const Mat& other) {
		delete[] data;

		this->rows = other.rows;
		this->cols = other.cols;
		this->type = other.type;

		int length = bytesPerPixel(type) * rows * cols;
		data = new unsigned char[length];

		unsigned char* src = other.data;
		unsigned char* dst = this->data;

		++length;
		while (--length)
			*(dst++) = *(src++);
	}

	void Mat::create(int rows, int cols, ImageType type) {
		delete[] data;

		this->rows = rows;
		this->cols = cols;
		this->type = type;
		int length = bytesPerPixel(type) * rows * rows;
		data = new unsigned char[length];
		std::fill(data, data + length, 0);
	}

	Color Mat::get(int i, int j) const {
		int pixelSize = bytesPerPixel(type);
		unsigned char* loc = data + pixelSize * j + pixelSize * i * rows;

		float r, g, b;
		switch (type) {
		default:
			throw "get(i, j) is missing the implementation for type '???'";
		case IM_8UC1:
			return Color(*loc);
		case IM_8UC3:
			r = *loc;
			g = ++(*loc);
			b = ++(*loc);

			return Color(r, g, b);
		case IM_32SC1:
			unsigned int i;
			i = ((unsigned int)*loc << 24);
			i |= ((unsigned int)*(++loc) << 16);
			i |= ((unsigned int)*(++loc) << 8);
			i |= *(++loc);
			float f = *reinterpret_cast<float*>(&i);

			return Color(f);
		}
	}
	Color Mat::get(Point index) const {
		return get(index.y, index.x);
	}

	void Mat::set(int i, int j, Color color) {
		int pixelSize = bytesPerPixel(type);
		unsigned char* loc = data + pixelSize * j + pixelSize * i * rows;

		switch (type) {
		default:
			throw "get(i, j) is missing the implementation for type '???'";
		case IM_8UC1:
			*loc = color.R;
			break;
		case IM_8UC3:
			*loc = color.R;
			*(++loc) = color.G;
			*(++loc) = color.B;
			break;
		case IM_32SC1:
			unsigned int i = *reinterpret_cast<unsigned int*>(&color.R);
			*loc = i >> 24;
			*(++loc) = (i >> 16) & 0xff;
			*(++loc) = (i >> 8) & 0xff;
			*(++loc) = i & 0xff;
			break;
		}
	}
	void Mat::set(Point index, Color color) {
		set(index.y, index.x, color);
	}

	void bitwise_and(const Mat& src1, const Mat& src2, Mat& dst) {
		unsigned char* pSrc1 = src1.data;
		unsigned char* pSrc2 = src2.data;
		unsigned char* pDst = dst.data;

		dst.create(src1.rows, src1.cols, src1.type);

		int i = src1.rows * src1.cols * bytesPerPixel(src1.type) + 1;
		while (--i) {
			*pDst++ = *pSrc1++ & *pSrc2++;
		}
	}

	void bitwise_or(const Mat& src1, const Mat& src2, Mat& dst) {
		unsigned char* pA = src1.data;
		unsigned char* pB = src2.data;
		unsigned char* pDst = dst.data;

		dst.create(src1.rows, src1.cols, src1.type);

		int i = src1.rows * src1.cols * bytesPerPixel(src1.type) + 1;
		while (--i) {
			*pDst++ = *pA++ | *pB++;
		}
	}

	void bitwise_xor(const Mat& src1, const Mat& src2, Mat& dst) {
		unsigned char* pA = src1.data;
		unsigned char* pB = src2.data;
		unsigned char* pDst = dst.data;

		dst.create(src1.rows, src1.cols, src1.type);

		int i = src1.rows * src1.cols * bytesPerPixel(src1.type) + 1;
		while (--i) {
			*pDst++ = *pA++ ^ *pB++;
		}
	}

	void threshold(const Mat& src, Mat& dst, int lowerbound, int upperbound) {
		unsigned char* pSrc = src.data;
		unsigned char* pDst = dst.data;

		dst.create(src.rows, src.cols, src.type);

		int i = src.rows * src.cols * bytesPerPixel(src.type) + 1;
		if (lowerbound <= upperbound) {
			while (--i) {
				*pDst++ = *pSrc >= lowerbound && *pSrc++ <= upperbound;
			}
		}
		else {
			while (--i) {
				*pDst++ = *pSrc >= lowerbound || *pSrc <= upperbound;
				++pSrc;
			}
		}
	}

	void split(const Mat& src, Mat dst[]) {
		switch (src.type)
		{
		default:
			throw "Not implemented type";
		case IM_8UC3:
			struct ColorC3 {
				unsigned char color[3];
			};

			for (int i = 0; i < 3; ++i) {
				dst[i].create(src.rows, src.cols, IM_8UC1);

				const ColorC3 *pSrc = reinterpret_cast<const ColorC3*>(src.data);
				unsigned char *pDst = dst[i].data;
				int j = src.rows * src.cols + 1;
				while (--j) {
					*pDst++ = (pSrc++)->color[i];
				}
				break;
			}
		}
	}

	Mat getRotationMatrix2D(Point2f center, float angle) {
		angle *= PI / 180;

		Mat R(3, 3, IM_32SC1);
		R.set(Point(0, 0), Color(cos(angle)));
		R.set(Point(0, 1), Color(-sin(angle)));
		R.set(Point(1, 1), Color(cos(angle)));
		R.set(Point(1, 0), Color(sin(angle)));

		R.set(Point(0, 2), Color(center.x - center.x * R.get(Point(0, 0)).R - center.y * R.get(Point(0, 1)).R));
		R.set(Point(1, 2), Color(center.y - center.x * R.get(Point(1, 0)).R - center.y * R.get(Point(1, 1)).R));
		R.set(Point(2, 2), Color(1));

		return R;
	}
	Mat getRotationMatrix2D(Point center, float angle) {
		return getRotationMatrix2D(Point2f(center.x, center.y), angle);
	}

	void warpAffine(const Mat& src, Mat& dst, const Mat& R, Point size) {
		if (size.x < 0)
			size.x = src.cols;
		if (size.y < 0)
			size.y = src.rows;

		Mat result(size.x, size.y, src.type);
		Point offset(size.x - src.cols, size.y - src.rows);

		for (int i = 0; i < src.rows; ++i) {
			for (int j = 0; j < src.cols; ++j) {
				int x = j - offset.x;
				int y = i - offset.y;
				int xDst = roundf(x * R.get(Point(0, 0)).R + y * R.get(Point(0, 1)).R + R.get(Point(0, 2)).R);
				int yDst = roundf(x * R.get(Point(1, 0)).R + y * R.get(Point(1, 1)).R + R.get(Point(1, 2)).R);

				if (xDst >= 0 && xDst < result.cols && yDst >= 0 && yDst < result.rows) {
					result.set(Point(xDst, yDst), src.get(i, j));
				}
			}
		}
		dst.copyFrom(result);
	}

	void warpAffine(const Point2f src, Point2f& dst, const Mat& R) {
		dst.x = src.x * R.get(Point(0, 0)).R + src.y * R.get(Point(0, 1)).R + R.get(Point(0, 2)).R;
		dst.y = src.x * R.get(Point(1, 0)).R + src.y * R.get(Point(1, 1)).R + R.get(Point(1, 2)).R;
	}
	void warpAffine(const Point& src, Point& dst, const Mat& R) {
		Point2f p(src.x, src.y);
		warpAffine(p, p, R);
		dst.x = roundf(p.x);
		dst.y = roundf(p.y);
	}

	void morphologyEx(const Mat& src, Mat& dst, Mor EDOC, int kernel)
	{
		dst.create(src.rows, src.cols, src.type);
		int Columns = src.cols;
		int Rows = src.rows;
		int k = kernel;
		if (k % 2 == 0)	{ k += 1; }
		int HalfKernel = k / 2;
		int MaskRow = 0, MaskColumns = 0;

		switch (EDOC) {
		case ERODE:
			for (Rows = HalfKernel; Rows < (src.rows - HalfKernel); Rows++){         // loop voor te behandele pixel  PAKT RAND NIET MEE
				for (Columns = (HalfKernel); Columns < (src.cols - HalfKernel); Columns++){
					MaskRow = Rows;
					MaskColumns = Columns;

					for (MaskRow - HalfKernel; MaskRow < (src.rows + HalfKernel); MaskRow++) {                    // Loop door masker
						for (MaskColumns - HalfKernel; MaskColumns < (src.cols + HalfKernel); MaskColumns++) {
							if (src.get(MaskRow, MaskColumns).R == 0){				// als de data in deze rij en colom 0 is
								dst.set(Rows, Columns, 0);						// maak de dstdata ook 0
							}
						}
					}
				}
			}

			break;

		case DILATE:
			for (Rows = HalfKernel; Rows < (src.rows - HalfKernel); Rows++){         // loop voor te behandele pixel  PAKT RAND NIET MEE
				for (Columns = (HalfKernel); Columns < (src.cols - HalfKernel); Columns++){
					MaskRow = Rows;
					MaskColumns = Columns;

					for (MaskRow - HalfKernel; MaskRow < (src.rows + HalfKernel); MaskRow++) {                    // Loop door masker
						for (MaskColumns - HalfKernel; MaskColumns < (src.cols + HalfKernel); MaskColumns++) {
							if (src.get(MaskRow, MaskColumns).R == 1){				// als de data in deze rij en colom 1 is
								dst.set(Rows, Columns, 1);						// maak de dstdata ook 1
							}
						}
					}
				}
			}

			break;

		case OPEN:
			// #warning create the Open function VisionOperators.cpp
			break;

		case CLOSE:
			// #warning create the Close function VisionOperators.cpp
			break;
		}
	}

	void drawRect(const Mat& src, Mat& dst, const RotatadRect& rect, const Color& color) {
		Mat R = getRotationMatrix2D(rect.center, rect.angle);
		Point2f extends(rect.size.x * 0.5f, rect.size.y * 0.5f);
		Point bottomRight = Point(roundf(rect.center.x - extends.x), roundf(rect.center.y - extends.y));
		Point topLeft = Point(roundf(rect.center.x + extends.x), roundf(rect.center.y + extends.y));
		if (&src != &dst)
			dst.copyFrom(src);

		for (int x = bottomRight.x; x < topLeft.x; ++x) {
			for (int y = bottomRight.y; y < topLeft.y; ++y) {
				Point p(x, y);
				warpAffine(p, p, R);
				if (p.x >= 0 && p.x < dst.cols && p.y >= 0 && p.y < dst.rows)
					dst.set(p, color);
			}
		}
	}

	void distanceTransform(const Mat& src, Mat& dst, DistanceType type) {
		if (type != Pythagoras)
			throw "Missing a implementation for one of the DistanceTypes";

		if (dst.cols != src.cols || dst.rows != src.cols || dst.type != src.type)
			dst.create(src.rows, src.cols, src.type);

		int maxCicleSize = ceil(powf(src.cols * src.cols + src.rows + src.rows, 0.5f));
		for (int x = 0; x < src.cols; ++x) {
			for (int y = 0; y < src.rows; ++y) {
				if (src.get(Point(x, y)).R) {
					float distance = 0;
					for (int i = 1; !distance; ++i) {
						if (i > src.cols + src.rows) {
							distance = i;
							break;
						}

						float radians = 0;
						while (true) {
							int x_ = roundf(cos(radians)*i);
							int y_ = roundf(sin(radians)*i);

							int dx_ = x_ + (radians < PI - 0.001f ? -1 : 1);
							int dy_ = y_ + (fmod(radians + 0.5f*PI, TWO_PI) < PI - 0.001f ? 1 : -1);

							float radians_new;
							float distances[2];
							distances[0] = abs(i - powf(dx_*dx_ + y_*y_, 0.5f));
							distances[1] = abs(i - powf(x_*x_ + dy_*dy_, 0.5f));
							if ((distances[0] < 0.98f || distances[0] > 1.25f) && (distances[1] < 0.98f || distances[1] > 1.25f))
								radians_new = atan2(dy_, dx_);
							else if (distances[0] <= distances[1])
								radians_new = atan2(y_, dx_);
							else
								radians_new = atan2(dy_, x_);

							radians_new = fmod(radians_new + TWO_PI, TWO_PI);

							x_ = roundf(cos(radians_new) * i);
							y_ = roundf(sin(radians_new) * i);
							if (!(x_ + x < 0 || x_ + x >= src.cols || y_ + y < 0 || y_ + y >= src.rows) && !src.get(Point(x_ + x, y_ + y)).R) {
								distance = powf(x_*x_ + y_*y_, 0.5f);
							}

							if (radians_new < radians)
								break;

							radians = radians_new;
						}
					}
					dst.set(Point(x, y), Color(distance));
				}
			}
		}
	}
}