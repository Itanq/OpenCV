
#ifndef _CORNER_DETECTION_H
#define _CORNER_DETECTION_H

#include<vector>
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

class CornerDetect
{
    public:
        CornerDetect();
        ~CornerDetect();
        void show();
        int loadImage(char*pfile, int flag);
        // MOravec 角点检测算法
        void cacMoravec(int size, int threshold);
        // Harris 角点检测算法
        void cacHarris(int b, int k, double t);
        // Shi-Tomasi角点检测算法
        void cacShi_Tomasi();

    private:
        cv::Mat srcImage;
        cv::Mat grayImage;
        cv::Mat edgeImage;
        cv::Mat cornerImage;
};


#endif
