#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<fstream>
#include<iostream>

#include"kcftracker.hpp"
#include"sflight.hpp"

cv::Rect rect;

bool eventFlag = false;
void MouseEvent1(int event, int x, int y, int flags, void*data)
{
	static cv::Point start(-1, -1);
	static cv::Point end(-1, -1);
	cv::Mat tmpImage = *(cv::Mat*)(data);
	// 根据标志位判断当前按键是否有效
	if (eventFlag)
	{
		rect.x = MIN(start.x, x);
		rect.width = abs(x - start.x);
		rect.height = abs(y - start.y);
	}
	// 鼠标左键按下时
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		eventFlag = true;
		start = cv::Point(x, y);
		rect = cv::Rect(x, y, 0, 0);
	}
}

#define Test_vedio
#ifdef Test_vedio
int main()
{
    cv::Rect trect;
    cv::Rect srect;

    KCFTracker      myTracker;
    SurroundFlight  mySurrf;

    cv::Mat frameInfo;
    cv::VideoCapture capture("/home/lijiachao/Tianql/视频/测试/9.mp4");
    if(!capture.isOpened())
    {
        std::cout << " error of open the video " << std::endl;
        return -1;
    }

//    std::fstream in("./rect.txt", std::ios::in);
//    in>>rect.x >> rect.y >> rect.height >> rect.width;
    int cnt=0;
    float pre_time=0.0f;
    int frame_cnt = capture.get(CV_CAP_PROP_FRAME_COUNT);
    while(cnt < frame_cnt)
    {
        capture >> frameInfo;
        if(cnt==0)
        {
            cv::namedWindow("Video");
            cv::imshow("Video", frameInfo);
            cv::setMouseCallback("Video", MouseEvent1, &frameInfo);
            cv::waitKey(0);
            trect = rect;

            cv::setMouseCallback("Video", MouseEvent1, &frameInfo);
            cv::waitKey(0);
            srect = rect;

            myTracker.init(trect, frameInfo);
            mySurrf.init();
            pre_time = capture.get(CV_CAP_PROP_POS_MSEC);
        }
        else
        {
            trect = myTracker.update(frameInfo);
            cv::rectangle(frameInfo, trect, cv::Scalar(255,0,0),2,3,0);
            cv::imshow("Video", frameInfo);

            float time = capture.get(CV_CAP_PROP_POS_MSEC);
            mySurrf.update(frameInfo, srect, time);
            impr_fc_param fc = mySurrf.GetSFParam();
            srect.x += fc.x * (time-pre_time);
            srect.y += sin(fc.r) * srect.x;
            cv::rectangle(frameInfo, srect, cv::Scalar(0,255,0),2,3,0);

            cv::imshow("Video", frameInfo);
            
        }
        cv::waitKey(10);
        ++cnt;
    }
    capture.release();
    return 0;
}
#endif

#ifdef Test_image
int main()
{
    cv::Mat srcImage = cv::imread("a.jpg");
    if(srcImage.empty()) {std::cout << " Error of open image " << std::endl; return -1;}
    cv::namedWindow("SF");
    cv::imshow("SF", srcImage);
    cv::setMouseCallback("SF", MouseEvent1, &srcImage);
    cv::waitKey(0);

    SurroundFlight SFlight;
    SFlight.init();

    int cnt=0;
    while(cnt<1000001)
    {
        ++cnt;
        cv::rectangle(srcImage, rect, cv::Scalar(0,255,0),2,3,0);
        cv::imshow("SF", srcImage);
        cv::waitKey(10);
        SFlight.update(srcImage, rect, cnt);
    }
    return 0;
}
#endif
