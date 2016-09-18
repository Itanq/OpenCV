
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<iostream>

// ʹ�� ptr<type>(i) ������ Mat ����
cv::Mat ScanImageAndReduce1(cv::Mat& srcImage, int value=64)
{
	CV_Assert(srcImage.depth() != sizeof(uchar));
	int channels = srcImage.channels();
	int nRows = srcImage.rows;
	// ���¿�֪ ʹ�� ptr ���� Mat �������� Mat ��ÿһ���ֽ�
	int nCols = srcImage.cols * channels;
	if (srcImage.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}
	uchar* p;
	for (int i = 0; i < nRows; ++i)
	{
		p = srcImage.ptr<uchar>(i);
		for (int j = 0; j < nCols; ++j)
		{
			p[j] = p[j] / value*value + value / 2;
		}
	}
	return srcImage;
}
// ʹ�õ��������� Mat ����
cv::Mat ScanImageAndReduce2(cv::Mat& srcImage, int value = 64)
{
	CV_Assert(srcImage.depth() != sizeof(uchar));
	const int channels = srcImage.channels();
	if (channels == 1)
	{
		for (cv::MatIterator_<uchar> it = srcImage.begin<uchar>(), end = srcImage.end<uchar>(); it != end; ++it)
		{
			*it = *it / value*value + value / 2;
		}
	}
	else if (channels == 3)
	{
		for (cv::MatIterator_<cv::Vec3b> it = srcImage.begin<cv::Vec3b>(), end = srcImage.end<cv::Vec3b>(); it != end; ++it)
		{
			(*it)[0] = (*it)[0] / value*value + value / 2;
			(*it)[1] = (*it)[1] / value*value + value / 2;
			(*it)[2] = (*it)[2] / value*value + value / 2;
		}
	}
	return srcImage;
}

// ͨ�� at() ������ Mat_ ���������� Mat ����
cv::Mat ScanImageAndReduce3(cv::Mat& srcImage, int value = 64)
{
	CV_Assert(srcImage.depth() != sizeof(uchar));
	const int channels = srcImage.channels();
	if (channels == 1)
	{
		for (int i = 0; i < srcImage.rows; ++i)
			for (int j = 0; j < srcImage.cols; ++j)
				// at<type>() ��֧��Point���͵���������ʶһ����
				srcImage.at<uchar>(i, j) = srcImage.at<uchar>(cv::Point(i,j)) / value*value + value / 2;
	}
	else if (channels == 3)
	{
		// Mat_ ���Ϳ���ֱ��ͨ��()ָ������������
		// Mat ���Ͳ�����
		cv::Mat_<cv::Vec3b> _srcImage = srcImage;
		for(int i=0; i<srcImage.rows; ++i)
			for(int j=0; j<srcImage.cols; ++j)
			{

				_srcImage(i, j)[0] = _srcImage(i, j)[0] / value*value + value / 2;
				_srcImage(i, j)[1] = _srcImage(i, j)[1] / value*value + value / 2;
				_srcImage(i, j)[2] = _srcImage(i, j)[2] / value*value + value / 2;
			}
		srcImage = _srcImage;
	}
	return srcImage;
}
int main()
{
	cv::Mat ansImage;
	cv::Mat srcImage = cv::imread("dota2.png");
//	ansImage = ScanImageAndReduce1(srcImage, 64);
//	ansImage = ScanImageAndReduce2(srcImage, 64);
	ansImage = ScanImageAndReduce3(srcImage, 64);
	cv::imshow("ansIamge", ansImage);
	cv::waitKey(0);
	return 0;
}