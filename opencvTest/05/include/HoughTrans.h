
#ifndef _HOUGH_TRANS_H_
#define _HOUGH_TRANS_H_

#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

class HoughTrans
{
    public:
        HoughTrans();
        ~HoughTrans();
        void show();
        int loadImage(char* pfile, int flag);
        void HoughLineTrans(int houghSpace, int threshold);
        void opencvHoughTransLine(double rho, double theta, int threshold);
        void opencvHoughTransCircle(int method);


    private:

        int threshold;
        int houghSpace;

        cv::Mat srcImage;
        cv::Mat edgeImage;
        cv::Mat houghImage;

};

#endif



