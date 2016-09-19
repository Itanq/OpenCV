
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<iostream>

int main()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data) return -1;
	cv::imshow("srcImage", srcImage);

	int nRows = srcImage.rows;
	int nCols = srcImage.cols;

	// 定义仿射变换的两组三个顶点
	cv::Point2f srcPoint[3];
	cv::Point2f resPoint[3];

	srcPoint[0] = cv::Point2f(0, 0);
	srcPoint[1] = cv::Point2f(nCols - 1, 0);
	srcPoint[2] = cv::Point2f(0, nRows - 1);

	resPoint[0] = cv::Point2f(nCols*0.00, nRows*0.13);
	resPoint[1] = cv::Point2f(nCols*0.95, nRows*0.25);
	resPoint[2] = cv::Point2f(nCols*0.15, nRows*0.80);

	// 定义仿射变换矩阵 2x3
	cv::Mat warpMat(cv::Size(2,3), srcImage.type());
	cv::Mat resultImage = cv::Mat::zeros(nRows, nCols, srcImage.type());
	// 计算仿射变换矩阵
	warpMat = cv::getAffineTransform(srcPoint, resPoint);
	// 根据仿射变换矩阵计算图像的仿射变换
	cv::warpAffine(srcImage, resultImage, warpMat, resultImage.size());
	cv::imshow("resultImage", resultImage);

	// 旋转中心,旋转角度,缩放因子
	cv::Point center = cv::Point(resultImage.rows/2, resultImage.cols/2);
	double angle = -50.0f;
	double scale = 0.6;
	
	// 获取旋转矩阵
	cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);
	// 根据旋转矩阵做仿射变换
	cv::warpAffine(resultImage, resultImage, rot_mat, resultImage.size());

	cv::imshow("warp_rot", resultImage);
	cv::waitKey(0);
	return 0;
}