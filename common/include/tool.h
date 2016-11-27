#ifndef _TOOL_H_
#define _TOOL_H_

#include<cmath>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

class Tool
{
    public:
        Tool();
        ~Tool();
        cv::Rect GetRoi(cv::Mat Image);

    private:
        cv::Mat     m_Image;
        cv::Rect    m_roi;
        bool        eventFlags;
        void setImage(cv::Mat Image);
        void on_mouse(int event, int x, int y, int flags, void*data);
};


#endif
