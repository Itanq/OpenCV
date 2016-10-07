#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/photo/photo.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

// f(x,y)分别在x和y方向的一阶差分定义为:
// detaF(x) = f(x+1,y)-f(x,y)
// detaF(y) = f(x,y+1)-f(x,y)
void diffOperation(const cv::Mat srcImage, cv::Mat& edgeXImage, cv::Mat edgeYImage)
{
	cv::Mat tmpImage = srcImage.clone();
	int nrows = tmpImage.rows;
	int ncols = tmpImage.cols;
	for (int i = 0; i < nrows - 1; ++i)
	{
		for (int j = 0; j < ncols - 1; ++j)
		{
			// 计算垂直边边缘
			edgeXImage.at<uchar>(i, j) = abs(tmpImage.at<uchar>(i + 1, j) - tmpImage.at<uchar>(i, j));
			// 计算水平边边缘 
			edgeYImage.at<uchar>(i, j) = abs(tmpImage.at<uchar>(i, j + 1) - tmpImage.at<uchar>(i, j));
		}
	}
}

int main()
{
	cv::Mat srcImage = cv::imread("dota2.png", 0);
	if (!srcImage.data)
	{
		std::cout << "can't open the image" << std::endl;
		return -1;
	}
	cv::imshow("srcImage", srcImage);
	cv::Mat edgeXImage(srcImage.size(), srcImage.type());
	cv::Mat edgeYImage(srcImage.size(), srcImage.type());
	// 计算图像差分
	diffOperation(srcImage, edgeXImage, edgeYImage);
	cv::imshow("edgeXImage", edgeXImage);
	cv::imshow("edgeYImage", edgeYImage);
	cv::Mat edgeImage(srcImage.size(), srcImage.type());
	// 水平和垂直边缘图像叠加
	cv::addWeighted(edgeXImage, 0.5, edgeYImage, 0.5, 0.0, edgeImage);
	cv::imshow("edgeImage", edgeImage);
	cv::waitKey(0);
	return 0;
}
