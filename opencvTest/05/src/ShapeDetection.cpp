
#include "ShapeDetection.h"

ShapeDetect::ShapeDetect()
{
}

ShapeDetect::~ShapeDetect()
{
}

void ShapeDetect::show()
{
    if(srcImage.data) cv::imshow("srcImage", srcImage);
    if(edgeImage.data) cv::imshow("edgeImage", edgeImage);
    if(contourImage.data) cv::imshow("contourImage", contourImage);
    cv::waitKey(0);
}

int ShapeDetect::loadImage(char* pfile, int flag)
{
    srcImage = cv::imread(pfile, flag);
    if(srcImage.empty())
    {
        std::cout << " the image is empty " << std::endl;
        return -1;
    }
    return 0;
}


void ShapeDetect::opencvContours(int mode, int method, int thresh)
{
    cv::Mat grayImage;
    cv::cvtColor(srcImage, grayImage, CV_BGR2GRAY);
    cv::blur(grayImage, grayImage, cv::Size(3,3));
    cv::Canny(grayImage, edgeImage, thresh, thresh*2, 3);

    cv::findContours(edgeImage, contour, hierarchy, mode, method, cv::Point(0,0));

    contourImage = cv::Mat::zeros(edgeImage.size(), CV_8UC3);
    for(int i=0; i<contour.size(); ++i)
    {
        cv::Scalar color = cv::Scalar(rand()%255, rand()%255, rand()%255);
        drawContours(contourImage, contour, i, color, 2, 8, hierarchy, 0, cv::Point());
    }
}

void ShapeDetect::opencvConvexHull(int thresh, int max_thresh)
{
    cv::cvtColor(srcImage, grayImage, CV_BGR2GRAY);
    cv::blur(grayImage, grayImage, cv::Size(3,3));
    cv::namedWindow("source", CV_WINDOW_AUTOSIZE);
    cv::imshow("source", srcImage);
    cv::createTrackbar("Thresh:", "source", &thresh, max_thresh, thresh_callback);
    thresh_callback(0, 0);
    cv::waitKey(0);
}

void ShapeDetect::thresh_callback(int , void*)
{
    cv::Mat threMat;
    // 阈值化操作
    cv::threshold(grayImage, threMat, thresh, 255, cv::THRESH_BINARY);
    // 轮廓检测
    findContours(threMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));
    // 凸包及缺陷检测参数
    std::vector<std::vector<int>> intHull(contour.size());
    std::vector<std::vector<cv::Point>> pointHull(contour.size());
    std::vector<std::vector<cv::Vec4i>> hullDefect(contours.size());
    for(size_t i=0; i<contour.size(); ++i)
    {
        // point 类型凸包检测
        convexHUll(cv::Mat(contour[i]), pointHull[i], false);
        // int 类型凸包检测
        convexHull(cv::Mat(contour[i]), intHull[i], false);
        convexityDefectes(cv::Mat(contour[i], ))
    }
}
