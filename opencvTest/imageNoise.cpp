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

// 高斯噪声是指概率密度函数服从高斯分布的一类噪声,若随变量来自于独立处于(0,1)之间的均匀分布,则按下列计算的
// z0 z1 服从高斯分布.
//z0 = sqrt((-2)*ln(U1))*cos(2*pi*U2);
//z1 = sqrt((-2)*ln(U1))*sin(2*pi*U2);
double generatorGaussianNoise(double mu, double sigma)
{
	const double eps = std::numeric_limits<double>::min();
	static double z0, z1;
	static bool flag = true;
	if (!flag) return z1*sigma + mu;
	double u1, u2;
	do
	{
		u1 = rand() * (1.0f / RAND_MAX);
		u2 = rand() * (1.0f / RAND_MAX);
	} while (u1 < eps);
	z0 = sqrt((-2)*log(u1))*cos(2 * CV_PI*u2);
	z1 = sqrt((-2)*log(u1))*sin(2 * CV_PI*u2);
	return z0*sigma + mu;
}

// 添加高斯噪声
cv::Mat addGaussianNoise(cv::Mat src)
{
	cv::Mat resultImage = src.clone();
	int rows = src.rows;
	int cols = src.cols*src.channels();
	if (src.isContinuous())
	{
		cols *= rows;
		rows = 1;
	}
	for (int i = 0; i < rows; ++i)
	{
		uchar * pdata = resultImage.ptr<uchar>(i);
		for (int j = 0; j < cols; ++j)
		{
			int val = pdata[j] + generatorGaussianNoise(2, 0.8) * 32;
			if (val < 0) val = 0;
			if (val > 255) val = 255;
			pdata[j] = (uchar)val;
		}
	}
	return resultImage;
}

int main()
{
	cv::Mat srcImage = cv::imread("dota2.png", 0);
	if (!srcImage.data)
	{
		std::cout << "can't open the image" << std::endl;
		return -1;
	}
//	cv::Mat resultImage = addSaltNoise(srcImage, 5000);
	cv::Mat resultImage = addGaussianNoise(srcImage);
	cv::imshow("srcImage", srcImage);
	cv::imshow("resultImage", resultImage);
	cv::waitKey(0);

	return 0;
}
