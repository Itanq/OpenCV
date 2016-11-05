#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/photo/photo.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<cmath>
#include<cstdio>
#include<iostream>

cv::Mat img, inpaintMask;
cv::Point prevPt(-1, -1);

static void onMouse(int event, int x, int y, int flags, void*)
{
	if (event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON))
		prevPt = cv::Point(-1, -1);
	// 按下鼠标左键
	else if (event == CV_EVENT_LBUTTONDOWN)
		prevPt = cv::Point(x, y);
	// 鼠标移动且发生左键事件
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))
	{
		cv::Point curPt(x, y);
		if (prevPt.x < 0)
			prevPt = curPt;
		cv::line(inpaintMask, prevPt, curPt, cv::Scalar::all(255), 5, 8, 0);
		cv::line(img, prevPt, curPt, cv::Scalar::all(255), 5, 8, 0);
		prevPt = curPt;
		cv::imshow("srcImage", img);
	}
}

int main()
{
	cv::Mat srcImage = cv::imread("dota2.png");
	if (!srcImage.data)
	{
		std::cout << "can't open the image" << std::endl;
		return -1;
	}

	img = srcImage.clone();
	inpaintMask = cv::Mat::zeros(img.size(), CV_8U);
	cv::imshow("srcImage", img);
	cv::setMouseCallback("srcImage", onMouse, 0);
	while (1)
	{
		char c = (char)cv::waitKey(0);
		if (c == 27) break;
		if (c == 'r')
		{
			inpaintMask = cv::Scalar::all(0);
			srcImage.copyTo(img);
			cv::imshow("srcImage", img);
		}
		if (c == 'i' || c == ' ')
		{
			cv::Mat res;
			cv::inpaint(img, inpaintMask, res, 3, CV_INPAINT_TELEA);
			cv::imshow("resImage", res);
		}
	}
	cv::waitKey(0);
	return 0;
}
