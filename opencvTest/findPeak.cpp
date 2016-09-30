
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

void findPeak(cv::Mat srcImage, std::vector<int> &resultVec)
{
	cv::Mat vecMat;
	cv::Mat resultImage = srcImage.clone();
	int thresh = 20;
	// 阈值化操作
	cv::threshold(srcImage, srcImage, thresh, 255, CV_THRESH_BINARY);
	srcImage.convertTo(srcImage, CV_32FC1);
	// 计算垂直投影
	cv::reduce(srcImage, vecMat, 0, CV_REDUCE_SUM);
	float * iptr = ((float*)vecMat.data) + 1;
	std::vector<int> tmpVec(vecMat.cols - 1, 0);
	// 遍历求差分符号函数
	for (int i = 0; i < vecMat.cols - 1; ++i, ++iptr)
	{
		if (*(iptr + 1) - *iptr > 0)
			tmpVec[i] = 1;
		else if (*(iptr + 1) - *iptr < 0)
			tmpVec[i] = -1;
		else
			tmpVec[i] = 0;
	}
	// 遍历符号函数
	for (int i = tmpVec.size()-1; i >= 0; --i)
	{
		if (tmpVec[i] == 0 && i == tmpVec.size() - 1)
			tmpVec[i] = 1;
		else if (tmpVec[i] == 0)
		{
			if (tmpVec[i + 1] >= 0) tmpVec[i] = 1;
			else tmpVec[i] = -1;
		}
	}
	// 波峰判断输出
	for (std::vector<int>::size_type i = 0; i != tmpVec.size() - 1; ++i)
		if (tmpVec[i + 1] - tmpVec[i] == -2)
			resultVec.push_back(i + 1);
	// 输出波峰位置
	for (int i = 0; i < resultVec.size(); ++i)
	{
		std::cout << resultVec[i] << " ";
		for (int ii = 0; ii < resultImage.rows; ++ii)
			resultImage.at<uchar>(ii, resultVec[i]) = 255;
	}
	cv::imshow("resultImage", resultImage);
}

int main()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "can't open the image" << std::endl;
		return -1;
	}
	cv::imshow("srcImage", srcImage);
	std::vector<int> resultVec;
	findPeak(srcImage, resultVec);
	cv::waitKey(0);
	return 0;
}