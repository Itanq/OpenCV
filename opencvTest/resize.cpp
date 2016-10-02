#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

// cv::resize(src, dst, dstSize, interpolation)
// src 源图 dst 目标图
// dstSize 目标图大小： 
// interpolation 插值方式
void testResize(cv::Mat srcImage)
{
	cv::imshow("srcImage", srcImage);
	cv::Mat dstImage(srcImage.rows/2, srcImage.cols/2, srcImage.type());
	double tTime;
	// 线性插值
	tTime = (double)cv::getTickCount();
	const int nTimes = 100;
	for (int i = 0; i < nTimes; ++i)
		cv::resize(srcImage, dstImage, dstImage.size(), CV_INTER_LINEAR);
	tTime = 1000 * ((double)cv::getTickCount() - tTime) / cv::getTickFrequency();
	tTime /= nTimes;
	std::cout << "resize-linear: " << tTime << std::endl;
	cv::imshow("resize-linear", dstImage);

	// 最邻近插值
	tTime = (double)cv::getTickCount();
	for (int i = 0; i < nTimes; ++i)
		resize(srcImage, dstImage, dstImage.size(), CV_INTER_NN);
	tTime = 1000 * ((double)cv::getTickCount() - tTime) / cv::getTickFrequency();
	tTime /= nTimes;
	std::cout << "resize-nearest: " << tTime << std::endl;
	cv::imshow("resize-nearest", dstImage);

	// 区域插值
	tTime = (double)cv::getTickCount();
	for (int i = 0; i < nTimes; ++i)
		resize(srcImage, dstImage, dstImage.size(), CV_INTER_AREA);
	tTime = 1000 * ((double)cv::getTickCount() - tTime) / cv::getTickFrequency();
	tTime /= nTimes;
	std::cout << "resize-AREA: " << tTime << std::endl;
	cv::imshow("resize-AREA", dstImage);

	// 三次插值
	tTime = (double)cv::getTickCount();
	for (int i = 0; i < nTimes; ++i)
		resize(srcImage, dstImage, dstImage.size(), CV_INTER_CUBIC);
	tTime = 1000 * ((double)cv::getTickCount() - tTime) / cv::getTickFrequency();
	tTime /= nTimes;
	std::cout << "resize-CUBIC: " << tTime << std::endl;
	cv::imshow("resize-CUBIC", dstImage);

	// 三次插值
	tTime = (double)cv::getTickCount();
	for (int i = 0; i < nTimes; ++i)
		resize(srcImage, dstImage, dstImage.size(), CV_INTER_LANCZOS4);
	tTime = 1000 * ((double)cv::getTickCount() - tTime) / cv::getTickFrequency();
	tTime /= nTimes;
	std::cout << "resize-LANCZOS4: " << tTime << std::endl;
	cv::imshow("resize-LANCOS4", dstImage);

	cv::waitKey(0);
}
int main()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "can't open the image" << std::endl;
		return -1;
	}
	testResize(srcImage);
	return 0;
}