
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

/*

OTSU 阈值化算法:

1) 统计灰度级中每个像素在整幅图像中个数
2) 计算每个像素在整幅图像的分布概率
3) 对灰度级进行遍历搜素,计算当前灰度值下前景-背景类间概率
4) 通过目标函数计算出类内与类间方差下对应的阈值

*/
int OTSU(cv::Mat &srcImage)
{
	int nCols = srcImage.cols;
	int nRows = srcImage.rows;
	int threshold = 0;

	int nSumPix[256];
	float nProDis[256];

	memset(nSumPix, 0, sizeof(nSumPix));
	memset(nProDis, 0, sizeof(nProDis));

	// 统计灰度级中每个像素在整幅图像中的个数
	for (int i = 0; i < nRows; ++i)
		for (int j = 0; j < nCols; ++j)
			nSumPix[(int)srcImage.at<uchar>(i, j)]++;

	// 计算每个像素占图像中的概率分布
	for (int i = 0; i < 256; ++i)
		nProDis[i] = (float)nSumPix[i] / (nCols*nRows);

	float w0, w1, u0_tmp, u1_tmp, u0, u1, delta_tmp;
	double delta_max = 0.0;

	// 遍历灰度级像素值,计算出最大类间方差下的阈值
	for (int i = 0; i < 256; ++i)
	{
		w0 = w1 = u0_tmp = u1_tmp = u0 = u1 = delta_tmp = 0;
		for (int j = 0; j < 256; ++j)
		{
			// 背景部分
			if (j <= i)
			{
				// 当前 i 为分割阈值
				w0 += nProDis[j];
				u0_tmp += j*nProDis[j];
			}
			else // 前景部分
			{
				// 当前 i 分割阈值
				w1 += nProDis[j];
				u1_tmp += j*nProDis[j];
			}
		}
		// 分别计算各类的平均灰度
		u0 = u0_tmp / w0;
		u1 = u1_tmp / w1;
		delta_tmp = (float)(w0*w1*std::pow(u0 - u1, 2));
		// 找到最大类间方差的阈值
		if (delta_tmp > delta_max)
		{
			delta_max = delta_tmp;
			threshold = i;
		}
	}
	return threshold;
}

void BinaryImage(cv::Mat &srcGray, cv::Mat &dstImage, int ostuThreshold)
{
	for (int i = 0; i < srcGray.rows; ++i)
		for (int j = 0; j < srcGray.cols; ++j)
			if (srcGray.at<uchar>(i, j) > ostuThreshold)
				dstImage.at<uchar>(i, j) = 255;
}

void TestOTSU()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "Can't open the Image" << std::endl;
		return;
	}

	cv::Mat srcGray;
	cv::cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	cv::imshow("GrayImage", srcGray);

	// 调用OTSU算法得到OTSU阈值
	int otsuThreshold = OTSU(srcGray);

	std::cout << "otsuThreshold: " << otsuThreshold << std::endl;

	cv::Mat otsuResultImage = cv::Mat::zeros(srcGray.size(), CV_8UC1);

	BinaryImage(srcGray, otsuResultImage, otsuThreshold);

	cv::imshow("otsuResultImage", otsuResultImage);
	cv::waitKey(0);
}

// 固定阈值化,大于分割阈值怎么怎么样,小与分割阈值怎么怎么样
void TestcvThreshold()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "Can't open the image" << std::endl;
		return;
	}
	
	cv::Mat srcGray;
	cv::cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	cv::imshow("GrayImage", srcGray);
	cv::Mat dstImage;
	int thresh = 130;
	std::string type = "threshold_Type";
	cv::threshold(srcGray, dstImage, thresh, 255, CV_THRESH_BINARY);
	cv::imshow(type+"0", dstImage);
	cv::threshold(srcGray, dstImage, thresh, 255, CV_THRESH_BINARY_INV);
	cv::imshow(type+"1", dstImage);
	cv::threshold(srcGray, dstImage, thresh, 255, CV_THRESH_TRUNC);
	cv::imshow(type+"2", dstImage);
	cv::threshold(srcGray, dstImage, thresh, 255, CV_THRESH_TOZERO);
	cv::imshow(type+"3", dstImage);
	cv::threshold(srcGray, dstImage, thresh, 255, CV_THRESH_TOZERO_INV);
	cv::imshow(type+"4", dstImage);
	cv::waitKey(0);

}

// 自适应阈值化
// 自适应阈值化算法: CV_ADAPTIVE_THRESH_MEAN_C  CV_ADAPTIVE_THRESH_GAUSSIAN_C
// 阈值类型: CV_THRESH_BINARY CV_THRESH_BINARY_INV
void TestAdaptiveThreshold()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "Can't open the Image" << std::endl;
		return;
	}
	cv::Mat srcGray;
	cv::cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	cv::imshow("GrayImage", srcGray);
	cv::Mat dstImage;

	int blockSize = 5;
	int constValue = 10;
	const int maxValue = 255;

	cv::adaptiveThreshold(srcGray, dstImage, maxValue, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, blockSize, constValue);
	cv::imshow("dstImage", dstImage);
	cv::adaptiveThreshold(srcGray, dstImage, maxValue, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, blockSize, constValue);
	cv::imshow("dstImage1", dstImage);
	cv::waitKey(0);
}

// 双阈值化也固定阈值化差不多,只是它有两个分割阈值,在分割阈值之间怎么怎么样
// 在分割阈值之外怎么怎么样
void TestDoubleThreshold()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "Can't open the Image" << std::endl;
		return;
	}
	cv::Mat srcGray;
	cv::cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	cv::imshow("GrayImage", srcGray);

	const int maxValue = 255;
	int low_threshold = 150;
	int heigh_threshold = 210;
	
	cv::Mat dstImage1, dstImage2, dstImage;
	// 对小阈值进行阈值化操作
	cv::threshold(srcGray, dstImage1, low_threshold, maxValue, CV_THRESH_BINARY);
	// 对大阈值进行阈值化操作
	cv::threshold(srcGray, dstImage2, heigh_threshold, maxValue, CV_THRESH_BINARY_INV);
	// 矩阵与运算得到二值化结果
	cv::bitwise_and(dstImage1, dstImage2, dstImage);
	cv::imshow("dstImage", dstImage);
	cv::waitKey(0);

}
int main()
{
//	TestOTSU();
//	TestcvThreshold();
//	TestAdaptiveThreshold();
	TestDoubleThreshold();
	return 0;
}