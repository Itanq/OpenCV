
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

/*

OTSU ��ֵ���㷨:

1) ͳ�ƻҶȼ���ÿ������������ͼ���и���
2) ����ÿ������������ͼ��ķֲ�����
3) �ԻҶȼ����б�������,���㵱ǰ�Ҷ�ֵ��ǰ��-����������
4) ͨ��Ŀ�꺯���������������䷽���¶�Ӧ����ֵ

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

	// ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
	for (int i = 0; i < nRows; ++i)
		for (int j = 0; j < nCols; ++j)
			nSumPix[(int)srcImage.at<uchar>(i, j)]++;

	// ����ÿ������ռͼ���еĸ��ʷֲ�
	for (int i = 0; i < 256; ++i)
		nProDis[i] = (float)nSumPix[i] / (nCols*nRows);

	float w0, w1, u0_tmp, u1_tmp, u0, u1, delta_tmp;
	double delta_max = 0.0;

	// �����Ҷȼ�����ֵ,����������䷽���µ���ֵ
	for (int i = 0; i < 256; ++i)
	{
		w0 = w1 = u0_tmp = u1_tmp = u0 = u1 = delta_tmp = 0;
		for (int j = 0; j < 256; ++j)
		{
			// ��������
			if (j <= i)
			{
				// ��ǰ i Ϊ�ָ���ֵ
				w0 += nProDis[j];
				u0_tmp += j*nProDis[j];
			}
			else // ǰ������
			{
				// ��ǰ i �ָ���ֵ
				w1 += nProDis[j];
				u1_tmp += j*nProDis[j];
			}
		}
		// �ֱ��������ƽ���Ҷ�
		u0 = u0_tmp / w0;
		u1 = u1_tmp / w1;
		delta_tmp = (float)(w0*w1*std::pow(u0 - u1, 2));
		// �ҵ������䷽�����ֵ
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

	// ����OTSU�㷨�õ�OTSU��ֵ
	int otsuThreshold = OTSU(srcGray);

	std::cout << "otsuThreshold: " << otsuThreshold << std::endl;

	cv::Mat otsuResultImage = cv::Mat::zeros(srcGray.size(), CV_8UC1);

	BinaryImage(srcGray, otsuResultImage, otsuThreshold);

	cv::imshow("otsuResultImage", otsuResultImage);
	cv::waitKey(0);
}

// �̶���ֵ��,���ڷָ���ֵ��ô��ô��,С��ָ���ֵ��ô��ô��
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

// ����Ӧ��ֵ��
// ����Ӧ��ֵ���㷨: CV_ADAPTIVE_THRESH_MEAN_C  CV_ADAPTIVE_THRESH_GAUSSIAN_C
// ��ֵ����: CV_THRESH_BINARY CV_THRESH_BINARY_INV
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

// ˫��ֵ��Ҳ�̶���ֵ�����,ֻ�����������ָ���ֵ,�ڷָ���ֵ֮����ô��ô��
// �ڷָ���ֵ֮����ô��ô��
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
	// ��С��ֵ������ֵ������
	cv::threshold(srcGray, dstImage1, low_threshold, maxValue, CV_THRESH_BINARY);
	// �Դ���ֵ������ֵ������
	cv::threshold(srcGray, dstImage2, heigh_threshold, maxValue, CV_THRESH_BINARY_INV);
	// ����������õ���ֵ�����
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