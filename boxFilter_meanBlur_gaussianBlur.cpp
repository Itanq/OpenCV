
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<iostream>

cv::Mat image;
cv::Mat g_boxImage;
cv::Mat g_blurImage;
cv::Mat g_gaussianImage;

int g_boxFilterValue = 3;
int g_meanBlurValue = 3;
int g_gaussianBlurValue = 3;

static void on_BoxFilter(int, void*)
{
	cv::boxFilter(image, g_boxImage, -1, cv::Size(g_boxFilterValue, g_boxFilterValue),cv::Point(-1,-1));
	cv::imshow("boxFilter",g_boxImage);
}

static void on_MeanBlur(int, void*)
{
	cv::blur(image, g_blurImage, cv::Size(g_meanBlurValue, g_meanBlurValue),cv::Point(-1,-1));
	cv::imshow("meanBlur", g_blurImage);
}

static void on_GaussianBlur(int, void*)
{
	cv::GaussianBlur(image, g_gaussianImage, cv::Size(g_gaussianBlurValue*2+1, g_gaussianBlurValue*2+1),0,0);
	cv::imshow("gaussianBlur", g_gaussianImage);
}

int main()
{
	image = cv::imread("fruit.jpg");
	if (!image.data) return -1;
	cv::imshow("SourceImage", image);

	g_blurImage = image.clone();
	g_boxImage = image.clone();
	g_gaussianImage = image.clone();

	//  方框滤波
	cv::namedWindow("boxFilter");
	cv::createTrackbar("内核值", "boxFilter", &g_boxFilterValue, 40, on_BoxFilter);
	on_BoxFilter(g_boxFilterValue, 0);

	// 均值滤波
	cv::namedWindow("meanBlur");
	cv::createTrackbar("内核值", "meanBlur", &g_meanBlurValue, 40, on_MeanBlur);
	on_MeanBlur(g_meanBlurValue, 0);

	// 高斯滤波
	cv::namedWindow("gaussianBlur");
	cv::createTrackbar("内核值", "gaussianBlur", &g_gaussianBlurValue, 40, on_GaussianBlur);
	on_GaussianBlur(g_gaussianBlurValue, 0);

	cv::waitKey(0);

	return 0;
}



