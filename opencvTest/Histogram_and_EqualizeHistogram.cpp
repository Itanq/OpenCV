
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>


//ͨ��Ӧ����Ŀ����,��������,Ŀ���������ٵȳ�����
void Test_HSV_Histogram()
{
	// HSV ��ɫ�ռ���ɫ�� ���Ͷ� ���ȹ���
	cv::Mat srcImage = cv::imread("dota2.png");
	if (srcImage.empty())
	{
		std::cout << "Can't open the SrcImage \n" << std::endl;
		return;
	}
	cv::imshow("Image", srcImage);

	cv::Mat hsvMat;
	// ת����HSV�ռ�ͼ
	cv::cvtColor(srcImage, hsvMat, CV_BGR2HSV);
	// ��ʼ���ҶȽײ���
	int hbins = 20, sbins = 32;
	int histSize[] = { hbins, sbins };
	// �Ҷȷ�Χ����
	float hranges[] = { 0,180 };
	// ���Ͷȷ�Χ����
	float sranges[] = { 0,256 };
	const float* ranges[] = { hranges, sranges };

	cv::MatND hist;
	// ѡȡֱ��ͼͨ��
	int channels[] = { 0,1 };
	// ���㵱ǰͨ��ֱ��ͼ
	cv::calcHist(&hsvMat, 1, channels, cv::Mat(), hist, 2, histSize, ranges, true, false);
	double maxValue = 0;
	// �ҵ�ֱ��ͼ�����ֵ
	cv::minMaxLoc(hist, 0, &maxValue, 0, 0);
	int scalar = 10;

	cv::Mat histImage = cv::Mat::zeros(sbins*scalar, hbins * 10, CV_8UC3);
	// ���� H S ͨ��
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

// BGR ֱ��ͼ
void Test_BGR_Histogram()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "Cant't open the image!!!\n";
		return;
	}
	cv::imshow("SrcImage", srcImage);

	// ����ͼ��ĸ���ͨ��
	std::vector<cv::Mat> bgr_planes;
	split(srcImage, bgr_planes);

	// ��ʼ��ֱ��ͼ�ļ������
	int histSize = 256;
	float range[] = { 0,256 };
	const float* histRange = { range };
	bool uniform = true;
	bool accumulate = false;

	// �ֱ�������ͨ����ֱ��ͼ
	cv::Mat b_hist, g_hist, r_hist;
	cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// ����ֱ��ͼ�Ļ�ͼ����
	int hist_w = 640, hist_h = 512;
	int bin_w = cvRound((double)hist_w / histSize);

	cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX);
	cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX);
	cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX);
	// �ֱ��ִÿ��ͨ����ֱ��ͼ
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

// �Զ���ֱ��ͼ
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

	// ��ʼ��ֱ��ͼ����
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

// �Ҷ�ֱ��ͼ�ľ��⻯
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
	// ֱ��ͼ�ľ��⻯
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
	//  �������ͨ��
	std::vector<cv::Mat> bgr_plane;
	cv::split(srcImage, bgr_plane);
	// �Ը�ͨ���ֱ���о��⻯
	for (int i = 0; i < bgr_plane.size(); ++i)
		cv::equalizeHist(bgr_plane[i], bgr_plane[i]);
	// �ϲ�ͨ��
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