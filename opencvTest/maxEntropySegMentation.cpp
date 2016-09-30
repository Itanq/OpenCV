
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

// ���㵱ǰλ�õ�������
float caculateCurrentEntropy(cv::Mat hist, int threshold)
{
	float backgroudSum = 0, targetSum = 0;
	const float* pDataHist = (float*)hist.ptr<float>(0);
	for (int i = 0; i < 256; ++i)
	{
		if (i < threshold) backgroudSum += pDataHist[i]; // �ۼƱ���ֵ
		else targetSum += pDataHist[i]; // �ۼ�Ŀ��ֵ
	}
	float backgroundEntropy = 0, targetEntropy = 0;
	for (int i = 0; i < 256; ++i)
	{
		if (i < threshold) // ���㱳����
		{
			if (pDataHist[i] == 0) continue;
			float ratio = pDataHist[i] / backgroudSum;
			backgroundEntropy += -ratio * logf(ratio);
		}
		else // ����Ŀ����
		{
			if (pDataHist[i] == 0) continue;
			float ratio = pDataHist[i] / targetSum;
			targetEntropy += -ratio * logf(ratio);
		}
	}
	return backgroundEntropy + targetEntropy;
}

// �������ֵ�ָ�
cv::Mat maxEntroySegMentation(cv::Mat inputMat)
{
	// ��ʼ��ֱ��ͼ����
	const int channels[1] = { 0 };
	const int histSize[1] = { 256 };
	float pranges[2] = { 0,256 };
	const float* ranges[1] = { pranges };
	cv::MatND hist;
	cv::calcHist(&inputMat, 1, channels, cv::Mat(), hist, 1, histSize, ranges);

	float maxentropy = 0;
	int max_index = 0;
	cv::Mat result;
	// �����õ��������ֵ�ָ�������ֵ
	for (int i = 0; i < 256; ++i)
	{
		float cur_entropy = caculateCurrentEntropy(hist, i);
		if (cur_entropy > maxentropy)
		{
			maxentropy = cur_entropy;
			max_index = i;
		}
	}
	threshold(inputMat, result, max_index, 255, CV_THRESH_BINARY);
	return result;
};

void testMaxEntropySegMentation()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "can't open the image" << std::endl;
		return;
	}
	cv::Mat resultImage1 = maxEntroySegMentation(srcImage);
	cv::imshow("srcImage", srcImage);
	cv::imshow("resultImage-color", resultImage1);
	cv::Mat srcGray;
	cv::cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	cv::Mat resultImage2 = maxEntroySegMentation(srcGray);
	cv::imshow("srcGray", srcGray);
	cv::imshow("reslutImage-gray", resultImage2);
	cv::waitKey(0);
}

int main()
{
	testMaxEntropySegMentation();
	return 0;
}