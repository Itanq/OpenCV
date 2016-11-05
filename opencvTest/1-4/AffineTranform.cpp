
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<iostream>

int main()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data) return -1;
	cv::imshow("srcImage", srcImage);

	int nRows = srcImage.rows;
	int nCols = srcImage.cols;

	// �������任��������������
	cv::Point2f srcPoint[3];
	cv::Point2f resPoint[3];

	srcPoint[0] = cv::Point2f(0, 0);
	srcPoint[1] = cv::Point2f(nCols - 1, 0);
	srcPoint[2] = cv::Point2f(0, nRows - 1);

	resPoint[0] = cv::Point2f(nCols*0.00, nRows*0.13);
	resPoint[1] = cv::Point2f(nCols*0.95, nRows*0.25);
	resPoint[2] = cv::Point2f(nCols*0.15, nRows*0.80);

	// �������任���� 2x3
	cv::Mat warpMat(cv::Size(2,3), srcImage.type());
	cv::Mat resultImage = cv::Mat::zeros(nRows, nCols, srcImage.type());
	// �������任����
	warpMat = cv::getAffineTransform(srcPoint, resPoint);
	// ���ݷ���任�������ͼ��ķ���任
	cv::warpAffine(srcImage, resultImage, warpMat, resultImage.size());
	cv::imshow("resultImage", resultImage);

	// ��ת����,��ת�Ƕ�,��������
	cv::Point center = cv::Point(resultImage.rows/2, resultImage.cols/2);
	double angle = -50.0f;
	double scale = 0.6;
	
	// ��ȡ��ת����
	cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);
	// ������ת����������任
	cv::warpAffine(resultImage, resultImage, rot_mat, resultImage.size());

	cv::imshow("warp_rot", resultImage);
	cv::waitKey(0);
	return 0;
}