
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

/*
	距离计算函数
*/
// 欧几里得狙击
float calcEuclideanDistance(int x1, int y1, int x2, int y2)
{
	return sqrt((x1-x2)*(x1 - x2) + (x2-y2)*(x2 - y2));
}
// 棋盘距离
float calcChessboardDistance(int x1, int y1, int x2, int y2)
{
	int a = abs(x1 - y1);
	int b = abs(x2 - y2);
	return ( a > b ? a : b);
}
// 街区距离
float calcBlockDistance(int x1, int y1, int x2, int y2)
{
	return abs(x1 - x2) + abs(x2 - y2);
}

void distanceTrans(cv::Mat srcImage, cv::Mat &resultImage)
{
	CV_Assert(srcImage.data != NULL);
	cv::Mat srcGray, srcBinary;
	cv::cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	cv::threshold(srcGray, srcBinary, 100, 255, CV_THRESH_BINARY);
	cv::imshow("srcBinary", srcBinary);

	int rows = srcBinary.rows;
	int cols = srcBinary.cols;
	uchar* pDataOne;
	uchar* pDataTwo;
	float disPara = 0, fDisMin = 0;
	// f(q)=min(f(q),D(p,q)+f(q));
	// f(q)表示像素点q的像素值
	// D(p,q)表示像素点p与q的距离
	// 第一遍遍历,用左模板更新像素值
	for (int i = 1; i < rows-1; ++i)
	{
		pDataOne = srcBinary.ptr<uchar>(i);
		for (int j = 1; j < cols; ++j)
		{
			pDataTwo = srcBinary.ptr<uchar>(i - 1);
			disPara = calcEuclideanDistance(i, j, i - 1, j - 1);
			fDisMin = cv::min((float)pDataOne[j], disPara + pDataTwo[j]);

			pDataTwo = srcBinary.ptr<uchar>(i);
			disPara = calcEuclideanDistance(i, j, i, j - 1);
			fDisMin = cv::min(fDisMin, disPara + pDataTwo[j - 1]);
			
			pDataTwo = srcBinary.ptr<uchar>(i + 1);
			disPara = calcEuclideanDistance(i, j, i + 1, j - 1);
			fDisMin = cv::min(fDisMin, disPara + pDataTwo[j - 1]);
			pDataOne[j] = (uchar)cvRound(fDisMin);
		}
	}
	// 第二遍遍历,用右模板更新像素值
	for (int i = rows - 2; i > 0; --i)
	{
		pDataOne = srcBinary.ptr<uchar>(i);
		for (int j = cols - 2; j >= 0; --j)
		{
			pDataTwo = srcBinary.ptr<uchar>(i+1);
			disPara = calcEuclideanDistance(i, j, i + 1, j);
			fDisMin = cv::min((float)pDataOne[j], disPara + pDataTwo[j]);

			pDataTwo = srcBinary.ptr<uchar>(i);
			disPara = calcEuclideanDistance(i, j, i, j + 1);
			fDisMin = cv::min(fDisMin, disPara + pDataTwo[j + 1]);

			pDataTwo = srcBinary.ptr<uchar>(i - 1);
			disPara = calcEuclideanDistance(i, j, i - 1, j + 1);
			fDisMin = cv::min(fDisMin, disPara + pDataTwo[j + 1]);

			pDataOne[j] = (uchar)cvRound(fDisMin);
		}
	}
	resultImage = srcBinary.clone();
}


// opencv 自带的图像距离变换函数
// distanceTransform(src,dst,distanceType,maskSize)
void TestDistanceTransform(cv::Mat srcImage, cv::Mat &resultImage)
{
	cv::Mat srcBinary;
	cv::Mat srcGray;
	cv::cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	cv::threshold(srcGray, srcBinary, 100, 255, CV_THRESH_BINARY);
	cv::imshow("srcBinary", srcBinary);
	cv::distanceTransform(srcBinary, resultImage, CV_DIST_L2, CV_DIST_MASK_PRECISE);
	cv::normalize(resultImage, resultImage, 0, 1.0, cv::NORM_MINMAX);
}

int main()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "can't open the image" << std::endl;
		return -1;
	}

	cv::Mat resultImage;
//	distanceTrans(srcImage, resultImage);
	TestDistanceTransform(srcImage, resultImage);
	cv::imshow("resultImage", resultImage);
	cv::waitKey(0);
	return 0;
}


