
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>


// ͼ��Աȶ�����
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
		// �Աȶ�����ӳ��
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

	// ���·�ʽ���÷�ͨ�������ɴ���ͨ���Ͷ�ͨ��
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
			// �����������ʾ�Ȼ�����....
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

// �Ҷȼ��ֲ�,��ȡͼ��ĳЩ����Ҷȵ���������,�����޸�ĳЩ������ͼ������ȱ��
// ���ַ���:1. ������Ȥ��ӳ��Ϊͬһֵ,������Ȥ��ӳ��Ϊ��һЩֵ,�õ���ֵͼ
//			2. ������Ȥ�ĻҶ�ֵ�����С,������Ȥ�ı��ֲ���,���ǻҶ�ͼ����
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
			// 1. ��ֵӳ��
			if (pData[j] > controlMin) pData[j] = 255;
			else pData[j] = 0;

			// 2. ����ӳ��
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
	// ��ͨ������
	cv::Mat resultImage1 = grayLayered(srcGray);
	cv::imshow("srcGray", srcGray);
	cv::imshow("reslutImage1-gray", resultImage1);

	// ��ͨ������
	cv::Mat resultImage2 = grayLayered(srcImage);
	cv::imshow("srcImage", srcImage);
	cv::imshow("reslutImage2-color", resultImage2);
	cv::waitKey(0);
}

// ����ƽ��ֲ�
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
		// ��Ӧ����ƽ��㼶
		pixMin = pow(2, bit);
		pixMax = pow(2, bit + 1);
		for (int i = 0; i < rows; ++i)
		{
			uchar * psrc = srcImage.ptr<uchar>(i);
			uchar * pres = resultImage.ptr<uchar>(i);
			for (int j = 0; j < cols; ++j)
			{
				// ��Ӧ����ƽ����ֵ��
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
	// ��ͨ��
//	showBitPlane(srcImage);
	cv::Mat srcGray;
	cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	// ��ͨ��
	showBitPlane(srcGray);
}
int main()
{
//	TestContrastStretch();
//	TestGrayLayered();
	TestShowBitPlane();
	return 0;
}