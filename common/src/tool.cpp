
#include "tool.h"


template<typename T>
T mmin(T a, T b){return a < b ? a : b;}
T mmax(T a, T b){return a > b ? a : b;}

Tool::Tool()
{
    eventFlags = false;
}

Tool::~Tool()
{
}

cv::Rect Tool::GetRoi(cv::Mat Image)
{

}

void Tool::on_mouse(int event, int x, int y, int flags, void *data)
{
    cv::Mat tmpImage = m_Image.clone();
    if(eventFlags)
    {
         m_roi.x = mmin(startPoint.x, x);
         m_roi.width = abs(startPoint.x - x);
         m_roi.height = abs(startPoint.y - y);
    }
    if(event==CV_EVENT_LBUTTOWNDOWN)
    {
        eventFlags =true;
        startPoint = cv::Point(x,y);
        m_roi = cv::Rect(x,y,0,0);
    }
    else if(event==CV_EVENT_LBUTTOWNUP)
    {
        enventFlags = false;
        cv::rectangle(tmpImage, m_roi, cv::Scalar(0,255,0),2,3,0);
        cv::imshow("ROIING", tmpImage);
    }
    cv::waitKey(0);
}

void Tool::setImage(cv::Mat Image)
{
    m_Image = Image.clone();
}

cv::Rect Tool::GetRoi(cv::Mat Image)
{
    setImage(Image);
    cv::namedWindow("getRoi");
    cv::setMouseCallback("getRoi", on_mouse, 0);
    return m_roi;
}

