
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<iostream>

void onChangeTrackbar(int pos, void *data)
{
	cv::Mat srcImage = *(cv::Mat*)(data);
	cv::Mat dstImage;
	// ��ֵ��srcImage ,��ֵΪ��������ֵ
	cv::threshold(srcImage, dstImage, pos, 255, 0);
	cv::imshow("dyn_threshold", dstImage);
}

int main()
{
	cv::Mat srcImage = cv::imread("./dota2.png");
	if (!srcImage.data) return -1;

	cv::Mat srcGray = srcImage;
	// ת��Ϊ�Ҷ�ͼ
	cv::cvtColor(srcImage, srcGray, CV_RGB2GRAY);
	cv::namedWindow("dyn_threshold");
	cv::imshow("dyn_threshold", srcGray);
	// ����һ�������� ������srcGray������
	cv::createTrackbar("pos", "dyn_threshold", 0, 255, onChangeTrackbar, &srcGray);
	cv::waitKey(0);
	return 0;
}