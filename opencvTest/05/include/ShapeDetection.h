#ifndef _SHAPE_DETECT_H_
#define _SHAPE_DETECT_H_

#include<vector>
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>


class ShapeDetect
{
    public:
        ShapeDetect();
        ~ShapeDetect();
        void show();
        int loadImage(char* pfile, int flag);
        void opencvContours(int mode, int method, int thresh);
        void opencvConvexHull();
    private:

        std::vector<cv::Vec4i> hierarchy;
        std::vector<std::vector<cv::Point> > contour;

        cv::Mat srcImage;
        cv::Mat grayImage;
        cv::Mat edgeImage;
        cv::Mat contourImage;
};

#endif
