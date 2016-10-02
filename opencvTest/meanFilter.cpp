#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

// ��ֵ�˲�����:

// 1. ���������������
// ��Ŀ��ͼ��F(x,y)��������ֵ�ļ��㹫ʽΪ:
// F(x,y)=(f(x-1,y)+f(x+1,y)+f(x,y-1)+f(x,y+1))/4
cv::Mat myFilter2D(cv::Mat srcImage)
{
	const int nChannels = srcImage.channels();
	cv::Mat resultImage(srcImage.size(), srcImage.type());
	for (int i = 1; i < srcImage.rows - 1; ++i)
	{
		// ��ȡ����ָ��
		const uchar* pre = srcImage.ptr<uchar>(i - 1);
		const uchar* cur = srcImage.ptr<uchar>(i);
		const uchar* nex = srcImage.ptr<uchar>(i + 1);
		uchar* output = resultImage.ptr<uchar>(i);
		for(int j=nChannels; j<nChannels*(srcImage.cols-1); ++j)
		{ 
			// 4-�����ֵ����
			*output++ = static_cast<uchar>((cur[j - nChannels] + cur[j + nChannels] + pre[j] + nex[j]) / 4.0f);
		}
	}
	// �߽紦��
	//resultImage.row(0).setTo(cv::Scalar(0));
	//resultImage.row(resultImage.rows - 1).setTo(cv::Scalar(0));
	//resultImage.col(0).setTo(cv::Scalar(0));
	//resultImage.col(resultImage.cols - 1).setTo(cv::Scalar(0));

	return resultImage;
}

// 2. �Դ���ͼ���������.
// �����߹�ʽ,�ĳɾ������ʽ����:
// F(x,y)=f(x,y)*mask;
// mask[3,3]={{0,1,0},{1,0,1},{0,1,0}}/4;
cv::Mat filter2D_(cv::Mat srcImage)
{
	cv::Mat resultImage(srcImage.size(), srcImage.type());

	cv::Mat kern = (cv::Mat_<float>(3, 3) << 0, 1, 0, 1, 0, 1, 0, 1, 0) / 4.0f;

	filter2D(srcImage, resultImage, srcImage.depth(), kern);
	return resultImage;
}

int main()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "can't open the image" << std::endl;
		return -1;
	}
	
	cv::Mat resultImage1 = myFilter2D(srcImage);
	cv::Mat resultImage2 = filter2D_(srcImage);

	cv::imshow("srcImage", srcImage);
	cv::imshow("resultImage1", resultImage1);
	cv::imshow("resultImage2", resultImage2);
	cv::waitKey();
	return 0;
}