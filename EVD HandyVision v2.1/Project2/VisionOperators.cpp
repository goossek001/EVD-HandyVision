#include "VisionOperators.h"
#include <algorithm>

namespace vision {
	#define PI 3.14159265359f

	Point::Point(int x, int y) {
		this->x = x;
		this->y = y;
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

	Mat getRotationMatrix2D(Point center, float angle) {
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

	void warpAffine(const Mat& src, Mat& dst, const Mat& R, Point size) {
		if (size.x < 0)
			size.x = src.cols;
		if (size.y < 0)
			size.y = src.rows;

		Mat result(size.x, size.y, src.type);
		Point offset(size.x - src.cols, size.y - src.rows);

		for (int i = 0; i < result.rows; ++i) {
			for (int j = 0; j < result.cols; ++j) {
				int x = j - offset.x;
				int y = i - offset.y;
				int xDst = x * R.get(Point(0, 0)).R + y * R.get(Point(0, 1)).R + R.get(Point(0, 2)).R + 0.5f;
				int yDst = x * R.get(Point(1, 0)).R + y * R.get(Point(1, 1)).R + R.get(Point(1, 2)).R + 0.5f;

				if (xDst >= 0 && xDst < src.cols && yDst >= 0 && yDst < src.rows) {
					result.set(i, j, src.get(Point(xDst, yDst)));
				}
				else {
					result.set(i, j, Color(0));
				}
			}
		}
		dst.copyFrom(result);
	}
}