#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

// 椒盐噪声 就是黑白杂点...可采用中值滤波去除
cv::Mat addSaltNoise(cv::Mat src, int n)
{
	cv::Mat srcImage = src.clone();
	for (int i = 0; i < n; ++i)
	{
		int r = rand() % srcImage.rows;
		int c = rand() % srcImage.cols;
		if (srcImage.channels() == 1)
			srcImage.at<uchar>(r, c) = 255;
		else
		{
			srcImage.at<cv::Vec3b>(r, c)[0] = 255;
			srcImage.at<cv::Vec3b>(r, c)[1] = 255;
			srcImage.at<cv::Vec3b>(r, c)[2] = 255;
		}
	}
	return srcImage;
}

// 暴力方法,非常慢
cv::Mat myMedianBlur(cv::Mat src, const int ksize)
{
	cv::Mat dstImage = src.clone();
	std::vector<uchar> vList;
	// 窗内总元素的个数
	const int nPix = (ksize * 2 + 1)*(ksize * 2 + 1);
	for(int i=ksize; i<src.rows-ksize; ++i)
	{ 
		for (int j = ksize; j < src.cols - ksize; ++j)
		{
			// 遍历窗内元素
			for (int pi = i - ksize; pi <= i + ksize; ++pi)
			for (int pj = j - ksize; pj <= j + ksize; ++pj)
			vList.push_back(src.at<uchar>(pi, pj));
			// 窗内元素排序
			sort(vList.begin(), vList.end());
			// 提取中间元素作为当前元素值
			dstImage.at<uchar>(i, j) = vList[nPix / 2];
			vList.clear();
		}
	}
	return dstImage;
}

cv::Mat myGaussianBlur(cv::Mat src, const int kernel, double delta)
{
	cv::Mat dstImage = src.clone();
	// 核半径
	int kerR = kernel / 2;
	// 高斯核因子
	cv::Mat Kernel = cv::Mat_<double>(kernel, kernel);
	// 归一化参数
	double alpha = 1 / (2 * (22 / 7)*delta*delta);
	// 生成高斯内核
	for (int i = -kerR; i <= kerR; ++i)
		for (int j = -kerR; j <= kerR; ++j)
			Kernel.at<double>(i + kerR, j + kerR) = exp(-(((j*j) + (i*i)) / (delta*delta * 2)))*alpha;
	double pix;
	for(int i=kerR; i<src.rows-kerR; ++i)
		for (int j = kerR; j < src.cols - kerR; ++j)
		{
			// 滑窗搜索完成高斯核平滑
			pix = 0;
			for (int pi = -kerR; pix <= kerR; ++pi)
				for (int pj = -kerR; pj <= kerR; ++pj)
					pix += src.at<uchar>(i + pi, j + pj)*Kernel.at<double>(kerR + pi, kerR + pj);
			dstImage.at<uchar>(i - kerR, j - kerR) = pix;
		}
	return dstImage;
}

int main()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "can't open the image" << std::endl;
		return -1;
	}

	cv::Mat dstImage1, dstImage2, dstImage3, dstImage4, dstImage5;
	cv::boxFilter(srcImage, dstImage1, srcImage.depth(), cv::Size(5, 5));
	cv::imshow("srcImage", srcImage);
	cv::imshow("boxFilter", dstImage1);

	cv::blur(srcImage, dstImage2, cv::Size(5, 5));
	cv::imshow("blur", dstImage2);

	cv::Mat saltImage = addSaltNoise(srcImage, 3000);
	cv::medianBlur(saltImage, dstImage3, 3);
//	dstImage3 = myMedianBlur(srcImage, 3);
	cv::imshow("saltImage", saltImage);
	cv::imshow("saltImage->medianBlur", dstImage3);

//	dstImage4 = myGaussianBlur(srcImage, 3, 0);
	cv::GaussianBlur(srcImage, dstImage4, cv::Size(3, 3), 0, 0);
	cv::imshow("gaussianBlur", dstImage4);

	cv::bilateralFilter(srcImage, dstImage5, 7, 20.0, 2.0);
	cv::imshow("bilateraFilter", dstImage5);

	cv::waitKey(0);
	return 0;
}
