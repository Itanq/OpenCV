
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>


// g(x,y) = a*f(x,y)+b
// g(x,y) 输出图像, f(x,y) 输入图像
// a 对比度变化量
// b 亮度变化量
cv::Mat linearTransform(cv::Mat &srcImage, float a, float b)
{
	cv::Mat resultImage = cv::Mat::zeros(srcImage.size(), srcImage.type());
	if (srcImage.channels() == 1)
	{
		for (int i = 0; i < srcImage.rows; ++i)
			for (int j = 0; j < srcImage.cols; ++j)
				resultImage.at<uchar>(i, j) = static_cast<uchar>(a*srcImage.at<uchar>(i, j) + b);
	}
	else
	{
		for (int i = 0; i < srcImage.rows; ++i)
			for (int j = 0; j < srcImage.cols; ++j)
				for (int c = 0; c < 3; ++c)
					resultImage.at<cv::Vec3b>(i, j)[c] = static_cast<uchar>(a*srcImage.at<cv::Vec3b>(i, j)[c] + b);
	}
	return resultImage;
}
void TestLinearTransform()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "can't open the image" << std::endl;
		return ;
	}
	float a = 1.20;
	float b = 50.0;
	cv::Mat resultImage = linearTransform(srcImage, a, b);
	cv::imshow("srcImage", srcImage);
	cv::imshow("Linear-resultImage", resultImage);
	cv::waitKey(0);
}

// s = clog(1+r)
// s 输出图  c 常数  r 输入图
cv::Mat logTransform1(cv::Mat srcImage, float c)
{
	cv::Mat resultImage = cv::Mat::zeros(srcImage.size(), srcImage.type());
	// s = clog(l+r)
	cv::add(srcImage, cv::Scalar(1.0), srcImage);
	srcImage.convertTo(srcImage, CV_32F);
	cv::log(srcImage, resultImage);
	resultImage = c * resultImage;
	// 归一化
	cv::normalize(resultImage,resultImage, 0, 255, cv::NORM_MINMAX);
	cv::convertScaleAbs(resultImage, resultImage);
	return resultImage;
}
cv::Mat logTransform2(cv::Mat srcImage, float c)
{
	cv::Mat resultImage = cv::Mat::zeros(srcImage.size(), srcImage.type());
	srcImage.convertTo(resultImage, CV_32F);
	resultImage = resultImage + 1;
	cv::log(resultImage, resultImage);
	resultImage = c * resultImage;
	cv::normalize(resultImage,resultImage, 0, 255);
	cv::convertScaleAbs(resultImage, resultImage);
	return resultImage;
}
cv::Mat logTransform3(cv::Mat srcImage, float c)
{
	cv::Mat resultImage = cv::Mat::zeros(srcImage.size(), srcImage.type());
	double gray = 0.0f;
	for(int i=0; i<srcImage.rows; ++i)
		for (int j = 0; j < srcImage.cols; ++j)
		{
			gray = (double)srcImage.at<uchar>(i, j);
			gray = c*log((double)(1 + gray));
			resultImage.at<uchar>(i, j) = static_cast<uchar>(gray);
		}
	cv::normalize(resultImage, resultImage, 0, 255,cv::NORM_MINMAX);
	cv::convertScaleAbs(resultImage, resultImage);
	return resultImage;
}
void TestLogTransform()
{
	cv::Mat srcImage = cv::imread("dota2.png", 0); // 一灰度图加载
	if (!srcImage.data)
	{
		std::cout << "can't open the image" << std::endl;
		return;
	}
	float c = 1.2;
	cv::Mat resultImage = logTransform3(srcImage, c);
	cv::imshow("srcImage", srcImage);
	cv::imshow("resultImage", resultImage);
	cv::waitKey(0);
}

int main()
{
//	TestLinearTransform();
	TestLogTransform();
	return 0;
}