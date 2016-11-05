#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

// Ӧ�ø���Ҷ�������о������
cv::Mat convolution(cv::Mat srcImage, cv::Mat kernel)
{
	cv::Mat resultImage(abs(srcImage.rows - kernel.rows) + 1, abs(srcImage.cols - kernel.cols) + 1, srcImage.type());
	// ���㸵��Ҷ�任�ĳߴ�
	cv::Size dftSize;
	dftSize.width = cv::getOptimalDFTSize(srcImage.cols + kernel.cols - 1);
	dftSize.height = cv::getOptimalDFTSize(srcImage.rows + kernel.rows - 1);
	// ������ʱͼ��
	cv::Mat tmpA(dftSize, srcImage.type(), cv::Scalar::all(0));
	cv::Mat tmpB(dftSize, srcImage.type(), cv::Scalar::all(0));
	// ��ȡ����Ȥ����
//	cv::Mat roiA(tmpA, cv::Rect(0, 0, srcImage.cols, srcImage.rows));
//	cv::Mat roiB(tmpB, cv::Rect(0, 0, kernel.cols, kernel.rows));
//	srcImage.copyTo(roiA);
//	kernel.copyTo(roiB);

	// ����Ҷ�任
	cv::dft(tmpA, tmpA, 0, srcImage.rows);
	cv::dft(tmpB, tmpB, 0, kernel.rows);

	// ��Ƶ����ÿ��Ԫ�ؽ��г˷�����
	cv::mulSpectrums(tmpA, tmpB, tmpA, cv::DFT_COMPLEX_OUTPUT);
	// �任���
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

	// �������
	cv::Mat resultImage = convolution(srcImage, kernel);
	// ��һ��������
	cv::normalize(resultImage, resultImage, 0, 1, CV_MINMAX);

	cv::imshow("srcImage", srcImage);
	cv::imshow("resultImage", resultImage);
	cv::waitKey(0);

	return 0;
}