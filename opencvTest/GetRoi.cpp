
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<iostream>

cv::Rect Roirect;

void MouseEvent(int event, int x, int y, int flags, void*data)
{
	static cv::Point pre_pt(-1, -1);
	static cv::Point cur_pt(-1, -1);
	// ����������ʱ
	if (event == CV_EVENT_LBUTTONDOWN && flags)
		pre_pt = cv::Point(x, y);
	// ����ƶ�������������ʱ
	else if ((event == CV_EVENT_MOUSEMOVE) && (flags & CV_EVENT_LBUTTONDOWN))
		cur_pt = cv::Point(x, y);
	// �������ͷ�ʱ
	else if (event == CV_EVENT_LBUTTONUP)
	{
		cur_pt = cv::Point(x, y);
		int width = abs(cur_pt.x - pre_pt.x);
		int height = abs(cur_pt.y - pre_pt.y);
		if (width == 0 || height == 0)
			cv::destroyWindow("Dst");
		else if (pre_pt.x < cur_pt.x && pre_pt.y < cur_pt.y)
			Roirect = cv::Rect(pre_pt.x, pre_pt.y, width, height);
		else if (pre_pt.x > cur_pt.x && pre_pt.y < cur_pt.y)
			Roirect = cv::Rect(cur_pt.x, pre_pt.y, width, height);
		else if (pre_pt.x > pre_pt.x && pre_pt.y > cur_pt.y)
			Roirect = cv::Rect(cur_pt.x, cur_pt.y, width, height);
		else if (pre_pt.x<cur_pt.x && pre_pt.y>cur_pt.y)
			Roirect = cv::Rect(pre_pt.x, cur_pt.y, width, height);

		cv::Mat tmpImage = *(cv::Mat*)(data);
		// �������ο�
		cv::rectangle(tmpImage, Roirect, cv::Scalar(255, 0, 0));
		cv::imshow("ROIing", tmpImage);
		cv::waitKey(0);

		cv::Mat img = tmpImage(Roirect);
		cv::imshow("GetRoi", img);
	}
}

bool eventFlag = false;
void MouseEvent1(int event, int x, int y, int flags, void*data)
{
	static cv::Point start(-1, -1);
	static cv::Point end(-1, -1);
	cv::Mat tmpImage = *(cv::Mat*)(data);
	// ���ݱ�־λ�жϵ�ǰ�����Ƿ���Ч
	if (eventFlag)
	{
		Roirect.x = MIN(start.x, x);
		Roirect.width = abs(x - start.x);
		Roirect.height = abs(y - start.y);
	}
	// ����������ʱ
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		eventFlag = true;
		start = cv::Point(x, y);
		Roirect = cv::Rect(x, y, 0, 0);
	}
	// �������ͷ�
	else if (event == CV_EVENT_LBUTTONUP)
	{
		eventFlag = false;
		cv::Mat roi = tmpImage.clone();
		// �������ο�
		cv::rectangle(tmpImage, Roirect, cv::Scalar(0, 255, 0), 2);
		cv::imshow("ROIing", tmpImage);
		cv::waitKey(0);
		cv::imshow("ROI", roi(Roirect));
	}
}

int main()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	cv::namedWindow("ROI");
	cv::imshow("ROI", srcImage);
	//cv::setMouseCallback("ROI", MouseEvent, &srcImage);
	cv::setMouseCallback("ROI", MouseEvent1, &srcImage);
	cv::waitKey(0);

	return 0;
}