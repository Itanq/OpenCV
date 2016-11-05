#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

#ifdef _cpp
void cppneihbourInterpolation(uint* srcImage, int srows, int scols, int channels, uint* dstImage, float yscalar, float xscalar)
{
	int index = 0;
	int drows = srows * yscalar;
	int dcols = scols * xscalar;
	for (int i = 0; i < drows; ++i)
	{
		int ir = cvFloor(i / yscalar);
		for (int j = 0; j < dcols; ++j)
		{
			int jc = cvFloor(j / xscalar);
			for(int ch=0; ch<channels; ++ch)
				dstImage[index++] = srcImage[ir*scols*channels + ch];
		}
	}
}
void saveFile(cv::Mat src, char* filename)
{
	std::fstream out(filename, std::ios::out);
	if (src.channels() == 1)
	{
		for (int i = 0; i < src.rows; ++i)
		{
			for (int j = 0; j < src.cols; ++j)
				out << src.at<uchar>(i, j) << " ";
			out << std::endl;
		}
	}
	else
	{
		for (int i = 0; i < src.rows; ++i)
		{
			for (int j = 0; j < src.cols; ++j)
				for (int ch = 0; ch < 3; ++ch)
					out << src.at<cv::Vec3b>(i, j)[ch];
			out << std::endl;
		}
	}
}
void testNeihbourInterpolationcpp(cv::Mat srcImage)
{
	int index = 0;
	uint * src = new uint[srcImage.rows*srcImage.cols*srcImage.channels()];
	uint * dst = new uint[srcImage.rows*srcImage.cols*srcImage.channels()];
	for (int i = 0; i < srcImage.rows; ++i)
	{
		uchar* pdata = srcImage.ptr<uchar>(i);
		for (int j = 0; j < srcImage.cols; ++j)
			src[index++] = pdata[j];
	}
	cv::Mat tmp(srcImage.rows, srcImage.cols, srcImage.type(), src);
	cppneihbourInterpolation(src, srcImage.rows, srcImage.cols, srcImage.channels(), dst, 0.6, 0.3);
	cv::Mat resultImage((int)srcImage.rows*0.6, (int)srcImage.cols*0.3, srcImage.type(), dst);
	cv::imshow("srcImage", srcImage);
	cv::imshow("dstImage", resultImage);
	cv::waitKey(0);
}
#endif

// �����ڽ���ֵ�ķ�ʽ��������,ԭ������ȡԴͼ�������������������ֵ��ΪĿ��ͼ��Ķ�Ӧ������ֵ
// ����Դͼ��F(x,y)�ֱ���ΪH*W,���ź��Ŀ��ͼ��G(x1,y1)�ֱ���Ϊh*w,�������ڽ���ֵ�Ĺ�ʽ����:
// x=int(x1*(float)W/w)
// y=int(y1*(float)H/h);
// ���е��������ӷֱ�Ϊyscalar��xscalar
cv::Mat neihbourInterpolation(cv::Mat srcImage, float yscalar, float xscalar)
{
	// Ŀ��������ź�Ĵ�С
	int dstrows = srcImage.rows*yscalar;
	int dstcols = srcImage.cols*xscalar;

	cv::Mat dstImage(dstrows, dstcols, srcImage.type(), cv::Scalar::all(0));
	for (int i = 0; i < dstrows; ++i)
	{
		// ��ӦԴͼ�������,ȡ��.
		int ir = cvFloor(i / yscalar);
		for (int j = 0; j < dstcols; ++j)
		{
			int jc = cvFloor(j / xscalar);
			// �߽紦��,��ָֹ��Խ��
			if (ir > srcImage.rows - 1) ir = srcImage.rows - 1;
			if (jc > srcImage.cols - 1) jc = srcImage.cols - 1;
			// ӳ��Դͼ��ֵ��Ŀ�������Ӧλ��
			if(srcImage.channels()==1)
				dstImage.at<uchar>(i, j) = srcImage.at<uchar>(ir, jc);
			else
				dstImage.at<cv::Vec3b>(i, j) = srcImage.at<cv::Vec3b>(ir, jc);
		}
	}
	return dstImage;
}
void testNeihbourInterpolation(cv::Mat srcImage)
{
	// ���Զ�ͨ��
	cv::Mat dstImage = neihbourInterpolation(srcImage, 0.5, 1.5);
	cv::imshow("srcImage", srcImage);
	cv::imshow("dstImage-color", dstImage);

	// ���Ե�ͨ��
	cv::Mat srcGray;
	cv::cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	cv::Mat resultImage = neihbourInterpolation(srcGray, 0.3, 0.8);
	cv::imshow("srcGray", srcGray);
	cv::imshow("reslutImage-gray", resultImage);

	cv::waitKey(0);
}

int main()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "can't open the image" << std::endl;
		return -1;
	}
	testNeihbourInterpolation(srcImage);
	return 0;
}
