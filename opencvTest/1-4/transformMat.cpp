
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<iostream>

void aboutMat()
{
	cv::Mat image1(cv::Size(4, 4), CV_8UC1, cv::Scalar(5));
	std::cout << "Image1 row: " << image1.rows << "\n" << "Image1 cols: " << image1.cols << std::endl;

	cv::Mat image2(cv::Size(4, 4), CV_32FC2, cv::Scalar(1, 5));
	std::cout << "Image2 rowRange(1,3):\n" << image2.rowRange(1, 3) << std::endl;
	std::cout << "Image2 colRange(2,4):\n" << image2.colRange(0, 4) << std::endl;

	image2.create(cv::Size(4, 4), CV_8UC3);
	std::cout << "Image2 channels: " << image2.channels() << std::endl;

	image2.convertTo(image2, CV_32F);
	std::cout << "Image2 depth(): " << image2.depth() << std::endl;

	cv::Mat image3 = cv::Mat::zeros(image2.rows, image2.cols,CV_8UC1);

	image1.row(3) = image1.row(0) * 2; // 可知行列数从0开始记
	std::cout << "Image1:\n" << image1 << std::endl;

	cv::Mat image4 = image2.col(3);
	std::cout << image4 << std::endl;

	image1.copyTo(image4); // 可知通道数不一样可以复制,复制后的通道数等于源矩阵的通道数
	std::cout << image4 << std::endl;
}

// 翻转图像
void aboutTransformMat1()
{
	cv::Mat srcImage = cv::imread("hore.jpg");
	if (srcImage.empty()) return;
	cv::imshow("srcImage", srcImage);
	
	cv::Mat resultImage(srcImage.size(), srcImage.type());
	cv::imshow("resualtImage", resultImage);
	cv::Mat xMapImage(srcImage.size(), CV_32FC1);
	cv::Mat yMapImage(srcImage.size(), CV_32FC1);
	int rows = srcImage.rows;
	int cols = srcImage.cols;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			xMapImage.at<float>(j,i) = cols - i;
			yMapImage.at<float>(j,i) = rows - j;
		}
	}
	cv::waitKey(0);
	cv::remap(srcImage, resultImage, xMapImage, yMapImage, CV_INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
	cv::imshow("srcImage", srcImage);
	cv::imshow("resultImage", resultImage);
	cv::waitKey(0);
}

// 平移操作 不改变图像大小
cv::Mat aboutTransformMat2(const char* img, int xOffset,int yOffset)
{
	cv::Mat srcImage = cv::imread(img);
	cv::Mat resualtImage(srcImage.size(), srcImage.type());
	for (int i = 0; i < srcImage.rows; ++i)
	{
		for (int j = 0; j < srcImage.cols; ++j)
		{
			int x = j - xOffset;
			int y = i - yOffset;
			if (x >= 0 && y >= 0 && x < srcImage.cols && y < srcImage.rows)
				resualtImage.at<cv::Vec3b>(i, j) = srcImage.ptr<cv::Vec3b>(y)[x];
		}
	}
	return resualtImage;
}

// 平移图像,改变大小
cv::Mat aboutTransformMat3(const char* img, int xOffset, int yOffset)
{
	cv::Mat srcImage = cv::imread(img);
	cv::Mat resualtImage(srcImage.size(), srcImage.type());

	int nRows = srcImage.rows + abs(yOffset);
	int nCols = srcImage.cols + abs(xOffset);

	for (int i = 0; i < srcImage.rows; ++i)
	{
		for (int j = 0; j < srcImage.cols; ++j)
		{
			int x = j - xOffset;
			int y = i - yOffset;
			if (x >= 0 && y >= 0 && x < nCols && y < nRows)
				resualtImage.at<cv::Vec3b>(i, j) = srcImage.ptr<cv::Vec3b>(y)[x];
		}
	}
	return resualtImage;
}

int main()
{
	cv::Mat ansImage;
	int xOffset = 50, yOffset = 80;
//	aboutTransformMat1();
//	ansImage = aboutTransformMat2("dota1.jpg", xOffset, yOffset);
//	cv::imshow("MoveLeft", ansImage);
//	cv::waitKey(0);
	
	ansImage = aboutTransformMat3("dota1.jpg", xOffset, yOffset);
	cv::imshow("MoveLeft", ansImage);
	cv::waitKey(0);
	
	return 0;
}
