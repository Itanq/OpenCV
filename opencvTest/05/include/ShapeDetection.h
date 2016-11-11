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
        // 轮廓
        void opencvContours(int mode, int method, int thresh);
        // 凸包及缺陷检测
        void opencvConvexHull(int thresh);
        // 最小矩形与圆生成
        void opencvClosingPoint();

        // 计算外接矩形与圆轮廓
        void cacBounding();
        // 计算任意方向外接矩形与圆轮廓
        void cacBoundRectAnyDirect();

    private:
        // 图像格式转换及阈值化
        void cvt_threh();

        std::vector<cv::Vec4i> hierarchy;
        std::vector<std::vector<cv::Point> > contours;

        cv::Mat srcImage;
        cv::Mat hullImage;
        cv::Mat grayImage;
        cv::Mat edgeImage;
        cv::Mat contourImage;
};

#endif
