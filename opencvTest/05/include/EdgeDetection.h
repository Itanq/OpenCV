
#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

class EdgeDetect
{
    public:
        EdgeDetect();
        ~EdgeDetect();
        void show();
        int loadImage(char* pfile, unsigned char flag);

        void sobelVerEdge();
        void DiffOperation();
        // 直接卷积实现sobel算子边缘监测
        void sobelEdge(unsigned char threshold);
        // 图像卷积实现sobel算子非极大值抑制
        void sobelOptEdge(int flag);
        // 拉普拉斯算子
        void laplaceEdge(unsigned char threshold);
        // roberts算子
        void robertsEdge();
        // Marr-Hildreth
        void marrHildrethEdge(int ker, double delta);

    private:
        cv::Mat srcImage;
        cv::Mat edgeImage;
};


#endif
