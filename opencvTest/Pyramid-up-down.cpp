#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

void pyramid(cv::Mat srcImage)
{
	if (srcImage.rows&2 || srcImage.cols&2)
		cv::resize(srcImage, srcImage, cv::Size(), 0.5, 0.5);
	else
		cv::resize(srcImage, srcImage, cv::Size(), 1, 1);
	cv::Mat pyrDownImage, pyrUpImage;
	// 向下采样 默认为1/2
	cv::pyrDown(srcImage, pyrDownImage);// , cv::Size(srcImage.cols / 2, srcImage.rows / 2));
	cv::imshow("pyrDown", pyrDownImage);
	// 向上采样 默认为 2倍
	cv::pyrUp(srcImage, pyrUpImage);// , cv::Size(srcImage.cols * 2, srcImage.rows * 2));
	cv::imshow("pyrUp", pyrUpImage);
	// 对下采样过程进行重构
	cv::Mat pyrBuildImage;
	cv::pyrUp(pyrDownImage, pyrBuildImage, cv::Size(pyrDownImage.cols * 2, pyrDownImage.rows * 2));
	cv::imshow("pyrBuildImage", pyrBuildImage);
	// 比较重构后的性能
	cv::Mat diffImage;
	cv::absdiff(srcImage, pyrBuildImage, diffImage);
	cv::imshow("diffImage", diffImage);

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
	pyramid(srcImage);
	return 0;
}