
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<iostream>

void onChangeTrackbar(int pos, void *data)
{
	cv::Mat srcImage = *(cv::Mat*)(data);
	cv::Mat dstImage;
	// 二值化srcImage ,阈值为进度条的值
	cv::threshold(srcImage, dstImage, pos, 255, 0);
	cv::imshow("dyn_threshold", dstImage);
}

int main()
{
	cv::Mat srcImage = cv::imread("./dota2.png");
	if (!srcImage.data) return -1;

	cv::Mat srcGray = srcImage;
	// 转化为灰度图
	cv::cvtColor(srcImage, srcGray, CV_RGB2GRAY);
	cv::namedWindow("dyn_threshold");
	cv::imshow("dyn_threshold", srcGray);
	// 创建一个进度条 来调整srcGray的数据
	cv::createTrackbar("pos", "dyn_threshold", 0, 255, onChangeTrackbar, &srcGray);
	cv::waitKey(0);
	return 0;
}