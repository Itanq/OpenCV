
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>


//通常应用在目标检测,特征分析,目标特征跟踪等场景中
void Test_HSV_Histogram()
{
	// HSV 颜色空间有色度 饱和度 亮度构成
	cv::Mat srcImage = cv::imread("dota2.png");
	if (srcImage.empty())
	{
		std::cout << "Can't open the SrcImage \n" << std::endl;
		return;
	}
	cv::imshow("Image", srcImage);

	cv::Mat hsvMat;
	// 转换成HSV空间图
	cv::cvtColor(srcImage, hsvMat, CV_BGR2HSV);
	// 初始化灰度阶参数
	int hbins = 20, sbins = 32;
	int histSize[] = { hbins, sbins };
	// 灰度范围设置
	float hranges[] = { 0,180 };
	// 饱和度范围设置
	float sranges[] = { 0,256 };
	const float* ranges[] = { hranges, sranges };

	cv::MatND hist;
	// 选取直方图通道
	int channels[] = { 0,1 };
	// 计算当前通道直方图
	cv::calcHist(&hsvMat, 1, channels, cv::Mat(), hist, 2, histSize, ranges, true, false);
	double maxValue = 0;
	// 找到直方图的最大值
	cv::minMaxLoc(hist, 0, &maxValue, 0, 0);
	int scalar = 10;

	cv::Mat histImage = cv::Mat::zeros(sbins*scalar, hbins * 10, CV_8UC3);
	// 遍历 H S 通道
	for (int h = 0; h < hbins; ++h)
	{
		for (int s = 0; s < sbins; ++s)
		{
			float binVal = hist.at<float>(h, s);
			int intensity = cvRound(binVal * 255 / maxValue);
			cv::rectangle(
				histImage,
				cv::Point(h*scalar, s*scalar),
				cv::Point((h + 1)*scalar - 1, (s + 1)*scalar - 1),
				cv::Scalar::all(intensity),
				CV_FILLED
			);
		}
	}
	cv::imshow("Source", srcImage);
	cv::imshow("H-S Histogram", histImage);
	cv::waitKey(0);
}

// BGR 直方图
void Test_BGR_Histogram()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "Cant't open the image!!!\n";
		return;
	}
	cv::imshow("SrcImage", srcImage);

	// 分离图像的各个通道
	std::vector<cv::Mat> bgr_planes;
	split(srcImage, bgr_planes);

	// 初始化直方图的计算参数
	int histSize = 256;
	float range[] = { 0,256 };
	const float* histRange = { range };
	bool uniform = true;
	bool accumulate = false;

	// 分别计算各个通道的直方图
	cv::Mat b_hist, g_hist, r_hist;
	cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// 设置直方图的绘图参数
	int hist_w = 640, hist_h = 512;
	int bin_w = cvRound((double)hist_w / histSize);

	cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX);
	cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX);
	cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX);
	// 分别回执每个通过的直方图
	for (int i = 1; i < histSize; ++i)
	{
		// blue 
		cv::line(
			histImage,
			cv::Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			cv::Point(bin_w*i, hist_h - cvRound(b_hist.at<float>(i))),
			cv::Scalar(255, 0, 0),
			2, 8, 0
		);
		// green
		cv::line(
			histImage,
			cv::Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			cv::Point(bin_w*i, hist_h - cvRound(g_hist.at<float>(i))),
			cv::Scalar(0, 255, 0),
			2, 8, 0
		);
		// red
		cv::line(
			histImage,
			cv::Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			cv::Point(bin_w*i, hist_h - cvRound(r_hist.at<float>(i))),
			cv::Scalar(0, 0, 255), 2, 8, 0
		);
	}
	cv::imshow("Histogram", histImage);
	cv::waitKey(0);
}

// 自定义直方图
void Test_Curstom_Histogram()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "Can't open the Image!!!\n";
		return;
	}
	cv::imshow("srcImage", srcImage);
	cv::Mat srcGray;
	cv::cvtColor(srcImage, srcGray, CV_BGR2GRAY);

	// 初始化直方图参数
	const int channels[1] = { 0 };
	const int histSize[1] = { 256 };
	float hranges[6] = { 0,60,120,160,220,255 };
	const float* ranges[1] = { hranges };

	cv::MatND hist;
	cv::calcHist(&srcImage, 1, channels, cv::Mat(), hist, 1, histSize, ranges);

	double maxHist;
	cv::minMaxLoc(hist, 0, &maxHist, 0, 0);
	//
	int hist_Size = hist.rows;
	cv::Mat histImg(hist_Size, hist_Size, CV_8U, cv::Scalar(255));
	for (int h = 0; h < hist_Size; ++h)
	{
		float binVal = hist.at<float>(h);
		int intensity = static_cast<int>(binVal*hist_Size / maxHist);
		cv::line(histImg, cv::Point(h, hist_Size), cv::Point(h, hist_Size - intensity), cv::Scalar::all(0));
	}
	cv::imshow("histImage", histImg);
	cv::waitKey(0);
	
}

// 灰度直方图的均衡化
void Test_Histogram_Equalize()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "Can't open the image" << std::endl;
		return;
	}
	cv::Mat srcGray;
	cv::cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	cv::imshow("SrcImage", srcImage);
	// 直方图的均衡化
	cv::Mat equalizeImg;
	cv::equalizeHist(srcGray, equalizeImg);
	cv::imshow("equalizeImage", equalizeImg);
	cv::waitKey(0);

}

void Test_RGB_Histogram_Equalize()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "Can't open the image" << std::endl;
		return;
	}
	cv::imshow("srcImage", srcImage);
	cv::Mat colorEqualize;
	//  分离各个通道
	std::vector<cv::Mat> bgr_plane;
	cv::split(srcImage, bgr_plane);
	// 对各通道分别进行均衡化
	for (int i = 0; i < bgr_plane.size(); ++i)
		cv::equalizeHist(bgr_plane[i], bgr_plane[i]);
	// 合并通道
	cv::merge(bgr_plane, colorEqualize);
	cv::imshow("colorEqualize", colorEqualize);
	cv::waitKey(0);

}
int main()
{
//	Test_HSV_Histogram();

//	Test_BGR_Histogram();

//	Test_Curstom_Histogram();

// 	Test_Histogram_Equalize();

	Test_RGB_Histogram_Equalize();

	return 0;
}