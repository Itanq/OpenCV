#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

// 应用傅里叶方法进行卷积操作
cv::Mat convolution(cv::Mat srcImage, cv::Mat kernel)
{
	cv::Mat resultImage(abs(srcImage.rows - kernel.rows) + 1, abs(srcImage.cols - kernel.cols) + 1, srcImage.type());
	// 计算傅里叶变换的尺寸
	cv::Size dftSize;
	dftSize.width = cv::getOptimalDFTSize(srcImage.cols + kernel.cols - 1);
	dftSize.height = cv::getOptimalDFTSize(srcImage.rows + kernel.rows - 1);
	// 创建临时图像
	cv::Mat tmpA(dftSize, srcImage.type(), cv::Scalar::all(0));
	cv::Mat tmpB(dftSize, srcImage.type(), cv::Scalar::all(0));
	// 获取感兴趣区域
//	cv::Mat roiA(tmpA, cv::Rect(0, 0, srcImage.cols, srcImage.rows));
//	cv::Mat roiB(tmpB, cv::Rect(0, 0, kernel.cols, kernel.rows));
//	srcImage.copyTo(roiA);
//	kernel.copyTo(roiB);

	// 傅里叶变换
	cv::dft(tmpA, tmpA, 0, srcImage.rows);
	cv::dft(tmpB, tmpB, 0, kernel.rows);

	// 对频谱中每个元素进行乘法操作
	cv::mulSpectrums(tmpA, tmpB, tmpA, cv::DFT_COMPLEX_OUTPUT);
	// 变换结果
	cv::dft(tmpA, resultImage, cv::DFT_INVERSE + cv::DFT_SCALE, resultImage.rows);

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

	cv::Mat kernel = (cv::Mat_<float>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	srcImage.convertTo(srcImage, CV_32F);

	// 卷积操作
	cv::Mat resultImage = convolution(srcImage, kernel);
	// 归一化结果输出
	cv::normalize(resultImage, resultImage, 0, 1, CV_MINMAX);

	cv::imshow("srcImage", srcImage);
	cv::imshow("resultImage", resultImage);
	cv::waitKey(0);

	return 0;
}