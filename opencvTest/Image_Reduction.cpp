
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/cvaux.h>
#include<opencv/highgui.h>
#include<iostream>

// 基于等间隔提取的图像缩放
cv::Mat imageReduction(cv::Mat &srcImage, float kx, float ky)
{
	int nRows = srcImage.rows * kx;
	int nCols = srcImage.cols * ky;
	cv::Mat resultImage(nRows, nCols, srcImage.type());
	for(int i=0; i<nRows; ++i)
	{
		for(int j=0; j<nCols; ++j)
		{
			int x = static_cast<int>((i+1)/kx)-1;
			int y = static_cast<int>((j+1)/ky)-1;
			resultImage.at<cv::Vec3b>(i,j) = srcImage.at<cv::Vec3b>(x,y);
		}
	}
	return resultImage;
}

// 求图像srcImage从左上角到右下角的区域的像素均值
cv::Vec3b areaAverage(const cv::Mat &srcImage, cv::Point_<int> leftPoint, cv::Point_<int> rightPoint)
{
	int tmp1 = 0, tmp2 = 0, tmp3 = 0;
	int nPix = (rightPoint.x-leftPoint.x+1)*(rightPoint.y-leftPoint.y+1);
	for(int i=leftPoint.x; i<=rightPoint.x; ++i)
	{
		for(int j=leftPoint.y; j<=rightPoint.y; ++j)
		{
			tmp1 += srcImage.at<cv::Vec3b>(i,j)[0];
			tmp2 += srcImage.at<cv::Vec3b>(i,j)[1];
			tmp3 += srcImage.at<cv::Vec3b>(i,j)[2];
		}
	}
	cv::Vec3b vecTmp;
	vecTmp[0] = tmp1/nPix;
	vecTmp[1] = tmp2/nPix;
	vecTmp[2] = tmp3/nPix;

	return vecTmp;
}

// 基于区域子块的图像缩放
cv::Mat imageReduction1(const cv::Mat &srcImage, double kx, double ky)
{
	int nRows = cvRound(srcImage.rows*kx);
	int nCols = cvRound(srcImage.cols*ky);
	cv::Mat resultImage(nRows, nCols, srcImage.type());

	int leftRowCoordinate = 0;
	int leftColCoordinate = 0;
	for(int i=0; i<nRows; ++i)
	{
		int x = static_cast<int>((i+1)/kx+0.5)-1;
		for(int j=0; j<nCols; ++j)
		{
			int y = static_cast<int>((j+1)/ky+0.5)-1;
			resultImage.at<cv::Vec3b>(i,j) = areaAverage(srcImage,cv::Point_<int>(leftRowCoordinate,leftColCoordinate),cv::Point_<int>(x,y));
			leftColCoordinate = y+1;
		}
		leftColCoordinate = 0;
		leftRowCoordinate = x+1;
	}
	return resultImage;
}

int main()
{
	cv::Mat ansImage;
	cv::Mat img = cv::imread("walker.jpg");
	cv::imshow("SrcImage", img);

	ansImage = imageReduction(img, 0.5, 0.5);
	cv::imshow("reduction0", ansImage);

	cv::Mat ansImage1 = imageReduction1(img, 0.5, 0.5);
	cv::imshow("reduction1", ansImage1);
	cv::waitKey(0);
	return 0;
}