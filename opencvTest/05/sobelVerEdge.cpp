#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui.highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

bool sobelVerEdge(cv::Mat srcImage, cv::Mat resultImage)
{
    CV_Assert(srcImage.channels()==1);
    srcImage.convertTo(srcImage, CV_32FC1);
    cv::Mat sobelX = (cv::Mat_<float>(3,3) << -0.125, 0, 0.125, -0.25, 0, 0.25, -0.125, 0, 0.125);
    cv::Mat conMat;
    cv::filter2D(srcImage, conMat, srcImage.type(), sobel);
    cv::Mat graMat;
    cv::multiply(conMat, conMat, graMat);
    
}
int main()
{
    cv::Mat srcImage = cv::imread("../image/home.jpg");
    cv::imshow("srcImage", srcImage);

    cv::waitKey(0);
    return 0;
}
