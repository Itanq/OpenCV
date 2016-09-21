
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/cvaux.h>
#include<opencv/highgui.h>
#include<algorithm>
#include<iostream>

cv::Mat angleRotate(cv::Mat &srcImage, int angle)
{
	float alpha = angle*CV_PI/180;
	// 构造旋转矩阵
	float rotateMat[3][3]={
		{cos(alpha),-sin(alpha),0},
		{sin(alpha), cos(alpha),0},
		{0,0,1}
	};
	int nSrcRows = srcImage.rows;
	int nSrcCols = srcImage.cols;
	// 计算旋转后图像的各个顶点的位置
	float a1 = nSrcCols * rotateMat[0][0];
	float b1 = nSrcCols * rotateMat[1][0];
	float a2 = nSrcCols * rotateMat[0][0] + nSrcRows * rotateMat[0][1];
	float b2 = nSrcCols * rotateMat[1][0] + nSrcRows * rotateMat[1][1];
	float a3 = nSrcRows * rotateMat[0][1];
	float b3 = nSrcRows * rotateMat[1][1];

	// 计算出极值点
	float kxMin = std::min(std::min(std::min(0.0f,a1),a2),a3);
	float kxMax = std::max(std::max(std::max(0.0f,a1),a2),a3);
	float kyMin = std::min(std::min(std::min(0.0f,b1),b2),b3);
	float kyMax = std::max(std::max(std::max(0.0f,b1),b2),b3);

	// 计算出旋转后的图像的大小
	int nRows = std::abs(kxMax-kxMin);
	int nCols = std::abs(kyMax-kyMin);
	
	cv::Mat dst(nRows, nCols, srcImage.type(),cv::Scalar::all(0));
	puts("dedededede");
	for(int i=0; i<nRows; ++i)
	{
		for(int j=0; j<nCols; ++j)
		{
			// 旋转坐标变换
			int x =  (j + kxMin) * rotateMat[0][0] - (i + kyMin) * rotateMat[0][1];
			int y = -(j + kxMax) * rotateMat[1][0] + (i + kyMin) * rotateMat[1][1];
			if(x>=0 && x<nSrcCols && y>=0 && y<=nSrcRows)
				dst.at<cv::Vec3b>(i,j) = srcImage.at<cv::Vec3b>(x,y);
		}
	}
	return dst;
}

int main()
{
	cv::Mat srcImage = cv::imread("a.jpg");
	if(!srcImage.data) return -1;

	cv::imshow("SrcImage", srcImage);
	int angle = 30;
	cv::Mat resultImage = angleRotate(srcImage, angle);
	cv::imshow("resultImage", resultImage);
	cv::waitKey(0);
	return 0;
}