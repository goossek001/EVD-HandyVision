#pragma once

#include <opencv2/opencv.hpp>

namespace vision {
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

		Mat();
		Mat(int rows, int cols, ImageType type);
		Mat(Point size, ImageType type);
		~Mat();

		void create(int rows, int cols, ImageType type);

		Color get(int i, int j);
		Color get(Point index);

		void set(int i, int j, Color color);
		void set(Point index, Color color);

		//TO WORK WITH OPENCV CODE
		operator cv::Mat() const {
			cv::Mat mat(rows, cols, type == IM_8UC1 ? CV_8UC1 : CV_8UC3);
			mat.data = data;
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j < cols; ++j) {
					switch (type) {
					default:
						throw "type not implemented";
					case IM_8UC1:
						mat.at<uchar>(i, j) = data[i*rows + j];
						break;
					case IM_8UC3:
						mat.at<float>(i, j) = data[3 * (i*rows + j)];
						break;
					}
				}
			}
			return mat;
		}
		//TO WORK WITH OPENCV CODE
		Mat(const cv::Mat& mat) : Mat(mat.rows, mat.cols, mat.type() == CV_8UC1 ? IM_8UC1 : IM_8UC3) {
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j < cols; ++j) {
					switch (type) {
					default:
						throw "type not implemented";
					case IM_8UC1:
						data[i*rows + j] = mat.at<uchar>(i, j);
						break;
					case IM_8UC3:
						data[3 * (i*rows + j)] = mat.at<float>(i, j);
						break;
					}
				}
			}
		}
	};

	void bitwise_and(const Mat& src1, const Mat& src2, Mat& dst);
	void threshold(const Mat& src, Mat& dst, int lowerbound, int upperbound);
	void split(const Mat& src, Mat dst[]);
}