
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>


// 图像对比度拉伸
cv::Mat contrastStretch(cv::Mat srcImage)
{
	cv::Mat resultImage = srcImage.clone();
	int pixMin = 256, pixMax = 0;
	/*
	if (resultImage.channels() == 1)
	{
		for (int i = 0; i < srcImage.rows; ++i)
		{
			uchar * pData = srcImage.ptr<uchar>(i);
			for (int j = 0; j < srcImage.cols; ++j)
			{
				if (pData[j] > pixMax) pixMax = pData[j];
				if (pData[j] < pixMin) pixMin = pData[j];
			}
		}
		// 对比度拉伸映射
		for (int i = 0; i < resultImage.rows; ++i)
		{
			uchar * pdata = resultImage.ptr<uchar>(i);
			for (int j = 0; j < resultImage.cols; ++j)
				pdata[j] = (pdata[j] - pixMin) * 255 / (pixMax - pixMin);
		}
	}
	else
	{
		for (int i = 0; i < srcImage.rows; ++i)
		{
			uchar * pdata = srcImage.ptr<uchar>(i);
			for (int j = 0; j < srcImage.cols; ++j)
			{
				for (int ch = 0; ch < srcImage.channels(); ++ch)
				{
					if (pdata[j*srcImage.channels()+ch] > pixMax) pixMax = pdata[j*srcImage.channels()+ch];
					if (pdata[j*srcImage.channels()+ch] < pixMin) pixMin = pdata[j*srcImage.channels()+ch];
				}
			}
		}
		for (int i = 0; i < resultImage.rows; ++i)
		{
			uchar* pdata = resultImage.ptr<uchar>(i);
			for (int j = 0; j < resultImage.cols; ++j)
				for (int ch = 0; ch < resultImage.channels(); ++ch)
					pdata[j*resultImage.channels() + ch] = (pdata[j*resultImage.channels() + ch] - pixMin) * 255 / (pixMax - pixMin);
		}
	}
	*/

	// 以下方式不用分通道数即可处理单通道和多通道
	int rows = srcImage.rows;
	int cols = srcImage.cols;
	if (resultImage.isContinuous())
	{
		cols *= rows;
		rows = 1;
	}

	for (int i = 0; i < rows; ++i)
	{
		uchar * pdata = resultImage.ptr<uchar>(i);
		for (int j = 0; j < cols; ++j)
		{
			if (pdata[j] > pixMax) pixMax = pdata[j];
			if (pdata[j] < pixMin) pixMin = pdata[j];
			// 下面这样访问竟然会出错....
//			if (resultImage.at<uchar>(i, j) > pixMax) pixMax = resultImage.at<uchar>(i, j);
//			if (resultImage.at<uchar>(i, j) < pixMin) pixMin = resultImage.at<uchar>(i, j);
		}
	}
	for (int i = 0; i < rows; ++i)
	{
		uchar * pdata = resultImage.ptr<uchar>(i);
		for (int j = 0; j < cols; ++j)
			pdata[j] = (pdata[j] - pixMin) * 255 / (pixMax - pixMin);
	}


	return resultImage;
}
void TestContrastStretch()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "Can't open the Image" << std::endl;
		return;
	}
	cv::Mat srcGray;
	cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	cv::Mat resultImage1 = contrastStretch(srcGray);
	cv::imshow("srcGray", srcGray);
	cv::imshow("resutlImage-gray", resultImage1);

	cv::Mat resultImage2 = contrastStretch(srcImage);
	cv::imshow("srcImage", srcImage);
	cv::imshow("resultImage-color", resultImage2);
	cv::waitKey(0);
}

// 灰度级分层,提取图像某些区域灰度的亮度特征,用于修复某些场景下图像质量缺陷
// 两种方法:1. 将感兴趣的映射为同一值,不感兴趣的映射为另一些值,得到二值图
//			2. 将感兴趣的灰度值变大或变小,不感兴趣的保持不变,还是灰度图不变
cv::Mat grayLayered(cv::Mat srcImage)
{
	cv::Mat resultImage = srcImage.clone();

	int rows = srcImage.rows; 
	int cols = srcImage.cols;

	if (resultImage.isContinuous())
	{
		cols *= rows;
		rows = 1;
	}
	
	int controlMin = 135;
	int controlMax = 200;
	for (int i = 0; i < rows; ++i)
	{
		uchar * pData = resultImage.ptr<uchar>(i);
		for (int j = 0; j < cols; ++j)
		{
			// 1. 二值映射
			if (pData[j] > controlMin) pData[j] = 255;
			else pData[j] = 0;

			// 2. 区域映射
			//if (pData[j] > controlMin && pData[j] < controlMax) pData[j] = controlMax;
		}
	}
	return resultImage;
}
void TestGrayLayered()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "can't open the image " << std::endl;
		return;
	}
	cv::Mat srcGray;
	cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	// 单通道测试
	cv::Mat resultImage1 = grayLayered(srcGray);
	cv::imshow("srcGray", srcGray);
	cv::imshow("reslutImage1-gray", resultImage1);

	// 多通道测试
	cv::Mat resultImage2 = grayLayered(srcImage);
	cv::imshow("srcImage", srcImage);
	cv::imshow("reslutImage2-color", resultImage2);
	cv::waitKey(0);
}

// 比特平面分层
void showBitPlane(cv::Mat srcImage)
{
	int rows = srcImage.rows;
	int cols = srcImage.cols;
	if (srcImage.isContinuous())
	{
		cols *= rows;
		rows = 1; 
	}

	int pixMin = 0;
	int pixMax = 0;

	cv::Mat resultImage = srcImage.clone();

	for (int bit = 0; bit < 8; ++bit)
	{
		// 相应比特平面层级
		pixMin = pow(2, bit);
		pixMax = pow(2, bit + 1);
		for (int i = 0; i < rows; ++i)
		{
			uchar * psrc = srcImage.ptr<uchar>(i);
			uchar * pres = resultImage.ptr<uchar>(i);
			for (int j = 0; j < cols; ++j)
			{
				// 相应比特平面层二值化
				if (psrc[j] > pixMin && psrc[j] < pixMax)
					pres[j] = 255;
				else
					pres[j] = 0;
			}
		}
		char windowName[20];
		sprintf(windowName, "BitNum%d", bit);
		cv::imshow(windowName, resultImage);
	}
	cv::waitKey(0);
}

void TestShowBitPlane()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "Cant't open the image" << std::endl;
		return;
	}
	// 多通道
//	showBitPlane(srcImage);
	cv::Mat srcGray;
	cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	// 单通道
	showBitPlane(srcGray);
}
int main()
{
//	TestContrastStretch();
//	TestGrayLayered();
	TestShowBitPlane();
	return 0;
}