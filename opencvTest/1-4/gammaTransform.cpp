
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>


cv::Mat gammaTransform(cv::Mat & srcImage, float kFactor)
{
	// 建立查找表文件
	unsigned char LUT[256];
	for (int i = 0; i < 256; ++i)
		LUT[i] = static_cast<uchar>(pow((float)(i / 255.0), kFactor)*255.0);
	cv::Mat resultImage = srcImage.clone();
	if (srcImage.channels() == 1)
	{
		cv::MatIterator_<uchar> iterator = resultImage.begin<uchar>();
		cv::MatIterator_<uchar> iteratorEnd = resultImage.end<uchar>();
		for (; iterator != iteratorEnd; ++iterator)
			*iterator = LUT[(*iterator)];
	}
	else
	{
		cv::MatIterator_<cv::Vec3b> iterator = resultImage.begin<cv::Vec3b>();
		cv::MatIterator_<cv::Vec3b> iteratorEnd = resultImage.end<cv::Vec3b>();
		for (; iterator != iteratorEnd; ++iterator)
		{
			(*iterator)[0] = LUT[(*iterator)[0]];
			(*iterator)[1] = LUT[(*iterator)[1]];
			(*iterator)[2] = LUT[(*iterator)[2]];
		}
	}
	return resultImage;
}

int main()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "Can't open the Image" << std::endl;
		return -1;
	}

	float kFactor1 = 0.3;
	float kFactor2 = 3.0;

	cv::Mat result1 = gammaTransform(srcImage, kFactor1);
	cv::Mat result2 = gammaTransform(srcImage, kFactor2);

	cv::imshow("srcImage", srcImage);
	cv::imshow("result1=0.3", result1);
	cv::imshow("result2=3.0", result2);
	cv::waitKey(0);

	return 0;
}