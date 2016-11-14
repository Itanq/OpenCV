
#ifndef _LICENSEPLATENUMBER_H_
#define _LICENSEPLATENUMBER_H_

#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>


class CarLicenseDetect
{
    public:
        CarLicenseDetect();
        ~CarLicenseDetect();
        void show() const;
        int loadImage(char* pfile, int flag);
        void carLicensePlateExtract();

    private:

        void RGB2HSV();
        void sobelVerEdge();
        void suspectedAreaExtract(cv::Mat& bw_edge);

        cv::Mat ansImage;
        cv::Mat edgeImage;
        cv::Mat grayImage;
        cv::Mat carSrcImage;
        cv::Mat carHSVImage;
        cv::Mat img_h, img_s, img_v;
};

#endif
