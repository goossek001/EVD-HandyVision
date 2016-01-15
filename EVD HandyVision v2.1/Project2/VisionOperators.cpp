#include "VisionOperators.h"
#include <algorithm>
#include <iostream>


namespace vision {
#define PI 3.14159265359f
#define TWO_PI 6.28318530718f

	Point::Point() {
		x = 0;
		y = 0;
	}
	Point::Point(const Point2f& v) {
		this->x = roundf(v.x);
		this->y = roundf(v.y);
	}
	Point::Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
	Point Point::operator*(const int& i) const {
		return Point(x * i, y * i);
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
	Point2f::Point2f(const Point& p) {
		this->x = p.x;
		this->y = p.y;
	}
	Point2f::Point2f(float x, float y) {
		this->x = x;
		this->y = y;
	}
	Point2f Point2f::operator*(const int& i) const {
		return Point2f(x * i, y * i);
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
		case IM_32FC1:
			return 4;
		}
	}

	Mat::Mat() : Mat(0, 0, IM_8UC1) {}

	ImageType convertType(int type) {
		switch (type) {
		default:
			throw "Unknown image type";
		case CV_8UC1:
			return IM_8UC1;
		case CV_8UC3:
			return IM_8UC3;
		case CV_32FC1:
			return IM_32FC1;
		}
	}
	int convertType(ImageType type) {
		switch (type) {
		default:
			throw "Unknown image type";
		case IM_8UC1:
			return CV_8UC1;
		case IM_8UC3:
			return CV_8UC3;
		case IM_32FC1:
			return CV_32FC1;
		}
	}
	Mat::Mat(const cv::Mat& cvMat) : Mat(cvMat.rows, cvMat.cols, convertType(cvMat.type())) {
		memcpy(data, cvMat.data, rows*cols * bytesPerPixel(type));
	}

	Mat::operator cv::Mat() {
		cv::Mat cvMat(rows, cols, convertType(type));
		memcpy(cvMat.data, data, rows*cols * bytesPerPixel(type));

		return cvMat;
	}

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

		unsigned char* pSrc = other.data;
		unsigned char* pDst = this->data;

		++length;
		while (--length)
			*(pDst++) = *(pSrc++);
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
			g = *(++loc);
			b = *(++loc);

			return Color(r, g, b);
		case IM_32FC1:
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
			*loc = (unsigned char)color.R;
			break;
		case IM_8UC3:
			*loc = (unsigned char)color.R;
			*(++loc) = (unsigned char)color.G;
			*(++loc) = (unsigned char)color.B;
			break;
		case IM_32FC1:
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

	void fill(Mat& img, Color color) {
		unsigned char* pImg = (unsigned char*)img.data;
		int i = img.rows * img.cols + 1;
		while (--i) {
			switch (img.type) {
			default:
				throw "ERROR";
			case IM_8UC1:
				*(pImg++) = color.R;
				break;
			case IM_8UC3:
				*(pImg++) = color.R;
				*(pImg++) = color.G;
				*(pImg++) = color.B;
				break;
			case IM_32FC1:
				unsigned int i = *reinterpret_cast<unsigned int*>(&color.R);
				*(pImg++) = i >> 24;
				*(pImg++) = (i >> 16) & 0xff;
				*(pImg++) = (i >> 8) & 0xff;
				*(pImg++) = i & 0xff;
				break;
			}
		}
	}

	Mat getRotationMatrix2D(Point2f center, float angle) {
		angle *= PI / 180;

		Mat R(3, 3, IM_32FC1);
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

		for (unsigned int i = 0; i < src.rows; ++i) {
			for (unsigned int j = 0; j < src.cols; ++j) {
				int x = j - offset.x;
				int y = i - offset.y;
				int xDst = (int)roundf(x * R.get(Point(0, 0)).R + y * R.get(Point(0, 1)).R + R.get(Point(0, 2)).R);
				int yDst = (int)roundf(x * R.get(Point(1, 0)).R + y * R.get(Point(1, 1)).R + R.get(Point(1, 2)).R);

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
		Point2f p((float)src.x, (float)src.y);
		warpAffine(p, p, R);
		dst.x = (int)roundf(p.x);
		dst.y = (int)roundf(p.y);
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
		Point bottomRight = Point((int)roundf(rect.center.x - extends.x), roundf(rect.center.y - extends.y));
		Point topLeft = Point((int)roundf(rect.center.x + extends.x), roundf(rect.center.y + extends.y));
		if (&src != &dst)
			dst.copyFrom(src);

		for (unsigned int x = bottomRight.x; x < topLeft.x; ++x) {
			for (unsigned int y = bottomRight.y; y < topLeft.y; ++y) {
				Point p(x, y);
				warpAffine(p, p, R);
				if (p.x >= 0 && p.x < dst.cols && p.y >= 0 && p.y < dst.rows)
					dst.set(p, color);
			}
		}
	}

	void distanceTransform(const Mat& src, Mat& dst) {
		for (unsigned int i = 0; i < src.rows; ++i) {
			for (unsigned int j = 0; j < src.cols; ++j) {
				if (!src.get(i, j).R && (!src.type == IM_8UC3 || (!src.get(i, j).G && !src.get(i, j).B)))
					dst.set(i, j, Color(0));
				else
					dst.set(i, j, Color(-1));
			}
		}
		for (unsigned int i = 0; i < src.rows; ++i) {
			for (unsigned int j = 0; j < src.cols; ++j) {
				if (!src.get(i, j).R && (!src.type == IM_8UC3 || (!src.get(i, j).G && !src.get(i, j).B)))
					dst.set(i, j, Color(0));
				if (!dst.get(i, j).R) {
					for (int i_kernel = -1; i_kernel <= 1; ++i_kernel) {
						for (int j_kernel = -1; j_kernel <= 1; ++j_kernel) {
							if (i_kernel + i >= 0 && i_kernel + i < src.rows && j_kernel + j >= 0 && j_kernel + j < src.rows) {
								if (dst.get(i + i_kernel, j + j_kernel).R >(unsigned int)abs(i_kernel) + (unsigned int)abs(j_kernel)) {
									dst.set(i + i_kernel, j + j_kernel, Color(abs(i_kernel) + (unsigned int)abs(j_kernel)));
								}
							}
						}
					}
				}
			}
		}
		bool change = true;
		while (change) {
			change = false;
			for (unsigned int i = 0; i < src.rows; ++i) {
				for (unsigned int j = 0; j < src.cols; ++j) {
					if (dst.get(i, j).R > 0) {
						for (int i_kernel = -1; i_kernel <= 1; ++i_kernel) {
							for (int j_kernel = -1; j_kernel <= 1; ++j_kernel) {
								if (i_kernel + i >= 0 && i_kernel + i < src.rows && j_kernel + j >= 0 && j_kernel + j < src.rows) {
									if (dst.get(i + i_kernel, j + j_kernel).R > dst.get(i, j).R + abs(i_kernel) + abs(j_kernel)) {
										dst.set(i + i_kernel, j + j_kernel, Color(dst.get(i, j).R + abs(i_kernel) + abs(j_kernel)));
										change = true;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void rgbtohsv(const Mat& src, Mat& dst) {
		unsigned char h, s, v;
		unsigned char r, g, b;
		unsigned char max, min, delta;
		int i;
		unsigned char* pSrc;
		unsigned char* pDst;
		int temp;

		if (src.type != dst.type || src.cols != src.cols || src.rows != src.rows)
			dst.create(src.rows, src.cols, src.type);

		pSrc = (unsigned char*)src.data;
		pDst = (unsigned char*)dst.data;
		i = src.rows * src.cols + 1;

		while (--i) {
			r = *(pSrc++);
			g = *(pSrc++);
			b = *(pSrc++);

			max = std::max(std::max(r, g), b);
			min = std::min(std::min(r, g), b);
			delta = max - min;

			//Calculate h
			if (!delta)
				h = 0;
			else {
				if (max == r)
					temp = (int)(60.0f / 360.0f*255.0f * (g / 255.0f - b / 255.0f) / (delta / 255.0f));
				else if (max == g)
					temp = (int)(60.0f / 360.0f*255.0f * ((b / 255.0f - g / 255.0f) / (delta / 255.0f) + 2.0f));
				else
					temp = (int)(60.0f / 360.0f*255.0f * ((r / 255.0f - g / 255.0f) / (delta / 255.0f) + 4.0f));
				if (temp < 0)
					temp += 360;
				h = (unsigned char)temp;
			}

			//Calculate s
			if (!max)
				s = 0;
			else
				s = delta * 255 / max;

			v = max;

			//Save pixel color in the data of dst
			*(pDst++) = h;
			*(pDst++) = s;
			*(pDst++) = v;
		}
	}

	void minMaxLoc(const Mat& src, int* min, Point* minLoc, int* max, Point* maxLoc) {
		unsigned int i, j, val;

		*min = src.get(0, 0).R;
		*minLoc = Point(0, 0);
		*max = src.get(0, 0).R;
		*maxLoc = Point(0, 0);

		for (int i = 1; i < src.rows; ++i) {
			for (int j = 1; j < src.cols; ++j) {
				val = src.get(i, j).R;
				if (val < *min) {
					*min = val;
					*minLoc = Point(j, i);
				}
				else if (val > *max) {
					*max = val;
					*maxLoc = Point(j, i);
				}
			}
		}
	}

	int labelBlobs(const Mat& src, Mat& dst, ConnectionType connected) {
		unsigned char* pSrc;
		unsigned char* pDst;
		unsigned int i, j, q, a;
		unsigned int lowestNeightborVal, pixelVal, labelVal;
		int blobCount;
		unsigned int connectionCount;
		bool needIteration;
		Point p;

		// Init variables
		connectionCount = (connected == FOUR ? 4 : 8);
		blobCount = 0;

		if (dst.type != src.type || dst.cols != src.cols || dst.rows != src.rows)
			dst.create(src.rows, src.cols, src.type);

		//Change all non zero pixels to value 255

		pSrc = (unsigned char*)src.data;
		pDst = (unsigned char*)dst.data;
		i = src.cols * src.rows + 1;
		while (--i) {
			lowestNeightborVal = (*pSrc & 0xff) ? 255 : 0;
			if ((*pSrc >> 8) & 0xff)
				lowestNeightborVal |= ((*pSrc >> 8) & 0xff) ? (255 << 8) : 0;
			if ((*pSrc >> 16) & 0xff)
				lowestNeightborVal |= ((*pSrc >> 16) & 0xff) ? (255 << 16) : 0;
			if (*pSrc >> 24)
				lowestNeightborVal |= (*pSrc >> 24) ? (255 << 24) : 0;
			*pDst = lowestNeightborVal;

			++pSrc; ++pDst;
		}

		// Label the blobs

		do {
			needIteration = 0;
			for (i = 0; i < dst.rows; ++i) {
				for (j = 0; j < dst.cols; ++j) {
					lowestNeightborVal = dst.get(i, j).R;
					//Look for the  neighbor with the lowest non-zero value
					for (q = 0; q < connectionCount; ++q) {
						switch (q) {
						case 0:
							p.x = -1;
							p.y = 0;
							break;
						case 1:
							p.x = 1;
							p.y = 0;
							break;
						case 2:
							p.x = 0;
							p.y = -1;
							break;
						case 3:
							p.x = 0;
							p.y = 1;
							break;
						case 4:
							p.x = -1;
							p.y = -1;
							break;
						case 5:
							p.x = 1;
							p.y = -1;
							break;
						case 6:
							p.x = -1;
							p.y = 1;
							break;
						case 7:
							p.x = 1;
							p.y = 1;
							break;
						}
						p.x += j;
						p.y += i;
						if (p.x >= 0 && p.x < dst.cols && p.y >= 0 && p.y < dst.rows &&
							dst.get(p.y, p.x).R && dst.get(p.y, p.x).R < lowestNeightborVal)
							lowestNeightborVal = dst.get(p.y, p.x).R;
					}
					// Label the pixel

					if (lowestNeightborVal == 255 && blobCount < 254)
						lowestNeightborVal = ++blobCount;
					if (dst.get(i, j).R != lowestNeightborVal) {
						if (dst.get(i, j).R != 255) { //Found a connection between two labels
							//Merge the two connected labels, by lowering the pixels value of all pixels with the same or higher value as the higher label
							labelVal = dst.get(i, j).R;
							a = dst.cols * dst.rows + 1;
							pDst = (unsigned char*)dst.data;
							while (--a)  {
								pixelVal = (*pDst) & 0xff;
								if (pixelVal >= labelVal && pixelVal != 255) {
									*pDst ^= pixelVal;
									if (pixelVal == labelVal) {
										*pDst |= lowestNeightborVal;
									}
									else {
										*pDst |= pixelVal - 1;
									}
								}
								pixelVal = (*pDst >> 8) & 0xff;
								if (pixelVal >= labelVal && pixelVal != 255) {
									*pDst ^= pixelVal << 8;
									if (pixelVal == labelVal) {
										*pDst |= lowestNeightborVal << 8;
									}
									else {
										*pDst |= (pixelVal - 1) << 8;
									}
								}
								pixelVal = (*pDst >> 16) & 0xff;
								if (pixelVal >= labelVal && pixelVal != 255) {
									*pDst ^= pixelVal << 16;
									if (pixelVal == labelVal) {
										*pDst |= lowestNeightborVal << 16;
									}
									else {
										*pDst |= (pixelVal - 1) << 16;
									}
								}
								pixelVal = (*pDst >> 24);
								if (pixelVal >= labelVal && pixelVal != 255) {
									*pDst ^= pixelVal << 24;
									if (pixelVal == labelVal) {
										*pDst |= lowestNeightborVal << 24;
									}
									else {
										*pDst |= (pixelVal - 1) << 24;
									}
								}
								++pDst;
							}
							--blobCount;
						}
						else
							dst.set(i, j, Color(lowestNeightborVal));

						needIteration = 1; //A nother iteration might be needed because changes where made and so the labeling was not perfect yet
					}
				}
			}
		} while (needIteration);

		return blobCount;
	}

#define QUEUESIZE 256
	// Mark all pixels that are zero and can be connected with the outer edge with other pixels with value zero
	void vMarkConnectedEmptySpace(Mat& img,
		unsigned int connectionCount,
		Point pos,
		Point* queue,
		unsigned int* queStart,
		unsigned int* queEnd,
		unsigned int queSize,
		unsigned int* needIteration) {
		Point p;
		unsigned int i;

		*needIteration = 0;

		if (img.get(pos.y, pos.x).R == 2) { //Check that the pixel is marked as 'waiting for processing'
			//Add the position to the queue
			queue[*queEnd] = pos;
			if (++(*queEnd) >= queSize)
				*queEnd = 0;
			//Process the objects that are in the queue
			while (1) {
				for (i = 0; i < connectionCount; ++i) { //Loop through the pixels around the pixel that is being processed
					switch (i) {
					case 0:
						p.x = -1;
						p.y = 0;
						break;
					case 1:
						p.x = 1;
						p.y = 0;
						break;
					case 2:
						p.x = 0;
						p.y = -1;
						break;
					case 3:
						p.x = 0;
						p.y = 1;
						break;
					case 4:
						p.x = -1;
						p.y = -1;
						break;
					case 5:
						p.x = 1;
						p.y = -1;
						break;
					case 6:
						p.x = -1;
						p.y = 1;
						break;
					case 7:
						p.x = 1;
						p.y = 1;
						break;
					}
					p.x += queue[*queStart].x;
					p.y += queue[*queStart].y;

					//When the neighbor pixel has value zero it will be queued to be processed in the futher
					if (p.x > 0 && p.x < img.cols - 1 &&
						p.y > 0 && p.y < img.rows - 1 &&
						!img.get(p.y, p.x).R) {
						if (*queStart != *queEnd) {
							queue[*queEnd] = p;
							if (++(*queEnd) >= queSize)
								*queEnd = 0;
						}
						else {    //The queue is full
							*needIteration = 1;
							img.set(p.y, p.x, Color(2));
						}
					}
				}
				//Change the processed pixel value
				//  to processed when all zero value neighbors where added to the queue
				//  to need to be processed when the queue was to small
				img.set(queue[*queStart].y, queue[*queStart].x, Color(*needIteration ? 2 : 3));

				//Remove the processed pixel from the queue
				if (++(*queStart) >= queSize)
					*queStart = 0;
				//Leave the loop when all pixels has been processed
				if (*queStart == *queEnd)
					break;
			}
		}
	}

	void fillHoles(const Mat& src, Mat& dst, ConnectionType connected) {
		Point queue[QUEUESIZE];
		unsigned int queStart, queEnd;
		unsigned int queSize;
		unsigned int i, j;
		unsigned int needIteration, needIterationTemp;
		unsigned int connectionCount;
		unsigned char* pDst;
		Point v;

		// Init variables

		connectionCount = (connected == FOUR ? 4 : 8);

		queStart = 0; queEnd = 0; queSize = QUEUESIZE;
		needIteration = 0;

		if (&src != &dst)
			dst.copyFrom(src);

		//Loop through all edge pixels and mark all pixels that are connected to the edge through zero value pixels

		//Loop through the pixels on the top and bottom side
		for (i = 0; i < dst.rows; ++i) {
			if (!dst.get(i, 0).R) {
				dst.set(i, 0, Color(2));
				v.x = 0;
				v.y = i;
				vMarkConnectedEmptySpace(dst, connectionCount, v, queue, &queStart, &queEnd, queSize, &needIterationTemp);
				if (needIterationTemp)
					needIteration = 1;
			}
			if (!dst.get(i, dst.cols - 1).R) {
				dst.set(i, dst.rows - 1, Color(2));
				v.x = dst.cols - 1;
				v.y = i;
				vMarkConnectedEmptySpace(dst, connectionCount, v, queue, &queStart, &queEnd, queSize, &needIterationTemp);
				if (needIterationTemp)
					needIteration = 1;
			}
		}
		//Loop through the pixels on the right and left side
		for (i = 0; i < src.cols; ++i) {
			if (!dst.get(0, i).R) {
				dst.set(0, i, Color(2));
				v.x = i;
				v.y = 0;
				vMarkConnectedEmptySpace(dst, connectionCount, v, queue, &queStart, &queEnd, queSize, &needIterationTemp);
				if (needIterationTemp)
					needIteration = 1;
			}
			if (!dst.get(dst.rows - 1, i).R) {
				dst.set(dst.rows - 1, i, Color(2));
				v.x = i;
				v.y = dst.rows - 1;
				vMarkConnectedEmptySpace(dst, connectionCount, v, queue, &queStart, &queEnd, queSize, &needIterationTemp);
				if (needIterationTemp)
					needIteration = 1;
			}
		}

		//Process the pixels that couldn't be processed because of a to small queue size
		while (needIteration) {
			for (i = 0; i < dst.rows; ++i) {
				for (j = 0; j < dst.cols; ++j) {
					v.x = j;
					v.y = i;
					vMarkConnectedEmptySpace(dst, connectionCount, v, queue, &queStart, &queEnd, queSize, &needIteration);
				}
			}
		}

		//Set zero pixels that aren't connected to the edge(value 0) to 1 and those who are(value > 1) to 0
		i = dst.rows * dst.cols + 1;
		pDst = (unsigned char*)dst.data;
		while (--i) {
			if (*pDst != 1) {
				if (*pDst)
					*pDst = 0;
				else
					*pDst = 1;
			}
			++pDst;
		}

		return;
	}

	void histogram(const Mat& src, unsigned char* hist, int *sum) {
		unsigned int i;
		unsigned char *h, *pSrc;
		unsigned int val;

		*sum = 0;
		pSrc = (unsigned char*)src.data;
		h = hist;

		i = 257;
		while (--i)
			*h++ = 0;


		i = 257;
		h = h;
		i = src.cols * src.rows + 1;
		while (--i) {
			(*sum) += *pSrc;
			++(hist[*pSrc++]);
		}

		return;
	}

	void blobAnalyse(const Mat& img, const int blobcount, BlobInfo* pBlobInfo)
	{
		int i, j, q, val;
		Rect_aabb* boundingRects;
		Point p;

		// Init variables

		boundingRects = (Rect_aabb*)malloc(32 * blobcount);

		i = blobcount;
		while (i--) {
			boundingRects[i].bottomLeft.x = img.cols;
			boundingRects[i].topRight.x = 0;
			boundingRects[i].bottomLeft.y = img.rows;
			boundingRects[i].topRight.y = 0;

			pBlobInfo[i].nof_pixels = 0;
			pBlobInfo[i].perimeter = 0;
		}

		//Loop through all pixels and adjust a blob info everytime a labeled is found
		i = img.rows;
		while (i--) {
			j = img.cols;
			while (j--) {
				val = img.get(i, j).R;
				if (img.get(i, j).R && img.get(i, j).R <= blobcount) {  //A labeled pixel is found
					--val;		//Go from blob nummer to his index int the blobinfo array

					// Adjust the number of pixels

					++(pBlobInfo[val].nof_pixels);

					// Adjust the bounding rect

					if (j < boundingRects[val].bottomLeft.x)
						boundingRects[val].bottomLeft.x = j;
					if (j > boundingRects[val].topRight.x)
						boundingRects[val].topRight.x = j;
					if (i < boundingRects[val].bottomLeft.y)
						boundingRects[val].bottomLeft.y = i;
					if (i > boundingRects[val].topRight.y)
						boundingRects[val].topRight.y = i;

					//Adjust the perimeter

					val = 0;
					q = img.rows;
					while (q--) {
						switch (q) {
						case 0:
							p.x = -1;
							p.y = 0;
							break;
						case 1:
							p.x = 1;
							p.y = 0;
							break;
						case 2:
							p.x = 0;
							p.y = -1;
							break;
						case 3:
							p.x = 0;
							p.y = 1;
							break;
						}
						p.x += j;
						p.y += i;

						if (!(p.x >= 0 && p.x < img.cols && p.y >= 0 && p.y < img.rows) ||
							img.get(p.y, p.x).R != img.get(i, j).R)
							++val;
					}
					if (val)
						pBlobInfo[(int)img.get(i, j).R - 1].perimeter += (val == 1 ? 1.0f : (val == 2 ? 1.414214f : 2.236068f));
				}
			}

		}

		//Calulate the height and width of the blob
		i = blobcount;
		while (i--) {
			pBlobInfo[i].width = boundingRects[i].topRight.x - boundingRects[i].bottomLeft.x + 1;
			pBlobInfo[i].height = boundingRects[i].topRight.y - boundingRects[i].bottomLeft.y + 1;
		}

		free(boundingRects);
	}

	bool inBound(const Mat& img, int i, int j) {
		return i >= 0 && j >= 0 && i < img.rows && j < img.cols;
	}

	bool inBound(const Mat& img, Point point) {
		return inBound(img, point.y, point.x);
	}

	void Rect_obb::vertices(Point arr[4]) {
		arr[0] = bottomLeft;
		arr[2] = topRight;

		Point2f p = Point2f(cos(radians), sin(radians));
		int i = dotProd(bottomLeft - topRight, p);

		arr[1] = bottomLeft + p;
		arr[3] = topRight - p;
	}

	int neighbourCount(const Mat& img, const unsigned char blobnr, int i, int j) {
		Point p;
		int neighbourCount = 0;

		int q = 4;
		while (q--) {
			switch (q) {
			case 3:
				p = Point(1, 0);
				break;
			case 2:
				p = Point(-1, 0);
				break;
			case 1:
				p = Point(0, 1);
				break;
			case 0:
				p = Point(0, -1);
				break;
			case 7:
				p = Point(1, 1);
				break;
			case 6:
				p = Point(-1, 1);
				break;
			case 5:
				p = Point(1, -1);
				break;
			case 4:
				p = Point(-1, -1);
				break;
			}
			p.x += j;
			p.y += i;

			if (inBound(img, p) && img.get(i, j).R == blobnr)
				++neighbourCount;
		}
		return neighbourCount;
	}

	int neighbourCount(const Mat& img, const unsigned char blobnr, Point p) {
		return neighbourCount(img, blobnr, p.y, p.x);
	}

	int findIndex(const std::vector<Point>& v, const Point& p) {
		int i = v.size();
		while (i--)
		if (v[i].x == p.x && v[i].y == p.y)
			return i;
		return -1;
	}

	std::vector<Point> findContour(const Mat& img, const unsigned char blobnr) {
		std::vector<Point> convexHull;
		int i, j;
		Point p, p2;
		bool l;
		int neighbours;
		int neighbourID;

		//Find the blob / his edge 
		i = img.rows;
		while (i--) {
			j = img.cols;
			while (j--) {
				if (img.get(i, j).R == blobnr) {
					convexHull.push_back(Point(j + 1, i));
					i = 0; j = 0;
				}
			}
		}

		//Loop through the edge of the blob
		l = true;
		while (l) {
			i = 4;
			while (i--) {
				switch (i) {
				case 2:
					p = Point(1, 0);
					break;
				case 0:
					p = Point(-1, 0);
					break;
				case 3:
					p = Point(0, 1);
					break;
				case 1:
					p = Point(0, -1);
					break;
				}
				p.x += convexHull[convexHull.size() - 1].x;
				p.y += convexHull[convexHull.size() - 1].y;

				if ((!inBound(img, p) || img.get(p).R != blobnr) && (convexHull.size() <= 1 || (p.x != convexHull[convexHull.size() - 2].x || p.y != convexHull[convexHull.size() - 2].y))) {
					// Check if the pixel has a neigbour
					neighbours = 0;
					j = 8;
					while (j--) {
						switch (j) {
						case 7:
							p2 = Point(1, 0);
							neighbourID = 1;
							break;
						case 6:
							p2 = Point(-1, 0);
							neighbourID = 2;
							break;
						case 5:
							p2 = Point(0, 1);
							neighbourID = 4;
							break;
						case 4:
							p2 = Point(0, -1);
							neighbourID = 8;
							break;
						case 3:
							p2 = Point(1, 1);
							neighbourID = 16;
							break;
						case 2:
							p2 = Point(-1, 1);
							neighbourID = 32;
							break;
						case 1:
							p2 = Point(1, -1);
							neighbourID = 64;
							break;
						case 0:
							p2 = Point(-1, -1);
							neighbourID = 128;
							break;
						}
						p2.x += p.x;
						p2.y += p.y;

						if (inBound(img, p2) && img.get(p2).R == blobnr) {
							neighbours |= neighbourID;
						}
					}
					if (neighbours && !((neighbours & 1) && (neighbours & 2)) && !((neighbours & 4) && (neighbours & 8))) {
						if (p.x == convexHull[0].x && p.y == convexHull[0].y) {
							l = false;
						}
						else
							convexHull.push_back(p);
						i = 0;
					}
				}
			}
		}
		return convexHull;
	}

	void convertToConvexHull(std::vector<Point>& contour) {
		bool l;
		int i;
		Point p1, p2, p3;
		float radians;

		l = true;
		while (l) {
			l = false;
			i = contour.size();

			while (i--) {
				p1 = contour[i ? i - 1 : contour.size() - 1];
				p2 = contour[i];
				p3 = contour[i == contour.size() - 1 ? 0 : i + 1];

				radians = atan2(p1.y - p2.y, p1.x - p2.x);
				radians -= atan2(p2.y - p3.y, p2.x - p3.x);
				radians = fmod(radians + TWO_PI, TWO_PI);

				if (radians <= PI) {
					contour.erase(contour.begin() + i);
					l = true;
					if (i)
						--i;
				}
			}
		}
	}

	float dotProd(Point2f v1, Point2f v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	Rect_obb findOMBB(const std::vector<Point>& convexHull) {
		Rect_obb rect, OMBB;
		int surface, smallestSurface;
		float radians;
		int min_x, max_x, min_y, max_y;
		int min_x_realcoord, max_x_realcoord, min_y_realcoord, max_y_realcoord;
		Point2f xAx, yAx;
		int i, j;
		float dot;

		smallestSurface = INT_MAX;

		i = convexHull.size();
		while (--i) {
			radians = atan2(convexHull[i - 1].y - convexHull[i].y, convexHull[i - 1].x - convexHull[i].x);

			xAx = Point2f(cos(radians), sin(radians));
			yAx = Point2f(-xAx.y, xAx.x);

			min_x = dotProd(xAx, convexHull[0]);
			max_x = dotProd(xAx, convexHull[0]);
			min_y = dotProd(yAx, convexHull[0]);
			max_y = dotProd(yAx, convexHull[0]);

			j = convexHull.size();
			while (--j) {
				dot = dotProd(xAx, convexHull[j]);
				if (dot < min_x)
					min_x = dot;
				else if (dot > max_x)
					max_x = dot;

				dot = dotProd(yAx, convexHull[j]);
				if (dot < min_y)
					min_y = dot;
				else if (dot > max_y)
					max_y = dot;
			}
			surface = (max_x - min_x) * (max_y - min_y);
			if (smallestSurface > surface) {
				OMBB.radians = radians;
				OMBB.bottomLeft = xAx * min_x + yAx * min_y;
				OMBB.topRight = xAx * max_x + yAx * max_y;

				smallestSurface = surface;
			}
		}

		return OMBB;
	}
}