
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<iostream>

cv::Mat srcImage;
// 鼠标事件
void MouseEvent(int event, int x, int y, int flags, void* param)
{
	char charText[30];
	cv::Mat tmpImg, hsvImg;
	tmpImg = srcImage.clone();
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		cv::Vec3b p = tmpImg.at<cv::Vec3b>(x, y);
		sprintf(charText, "R=%d, G=%d, R=%d", p[2], p[1], p[0]);
		// 写RGB信息文本到图像
		cv::putText(tmpImg, charText, cv::Point(8, 20), cv::FONT_HERSHEY_PLAIN, 2, CV_RGB(0, 0, 255));
		cv::imshow("Test", tmpImg);
		cv::imwrite("LbuttonDown.jpg", tmpImg);
	}
	else if(event==CV_EVENT_RBUTTONDOWN)
	{
		// 转换成 HSV 图
		cv::cvtColor(tmpImg, hsvImg, CV_BGR2HSV);
		cv::Vec3b p = hsvImg.at<cv::Vec3b>(x, y);
		sprintf(charText, "H=%d, S=%d, V=%d", p[0], p[1], p[2]);
		cv::putText(hsvImg, charText, cv::Point(8,20), cv::FONT_HERSHEY_PLAIN, 2, CV_RGB(0, 255, 0));
		cv::imshow("Test", hsvImg);
		cv::imwrite("RbuttonDown.jpg", hsvImg);
	}
	else
	{
		sprintf(charText, "X=%d, Y=%d", x, y);
		cv::putText(tmpImg, charText, cv::Point(8,20), cv::FONT_HERSHEY_PLAIN, 2, CV_RGB(255, 0, 0));
		cv::imshow("SrcImage", tmpImg);
	}
}

int main()
{
	srcImage = cv::imread("dota2.png");
	if (srcImage.empty())return -1;
	cv::namedWindow("srcImage");
	cv::setMouseCallback("srcImage", MouseEvent, 0);
	cv::waitKey(0);
	return 0;
}