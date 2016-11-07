#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

// 图像差分运算求边缘
void DiffOperation(cv::Mat srcImage, cv::Mat& edgeXImage, cv::Mat& edgeYImage)
{
    int rows = srcImage.rows-1;
    int cols = srcImage.cols-1;
    std::cout << " cols: " << cols << std::endl; 
    for(int i=0; i<rows; ++i)
    {
        for(int j=0; j<cols; ++j)
        {
            edgeXImage.at<uchar>(i, j) = abs(srcImage.at<uchar>(i+1,j)-srcImage.at<uchar>(i,j));
            edgeYImage.at<uchar>(i, j) = abs(srcImage.at<uchar>(i,j+1)-srcImage.at<uchar>(i,j));
        }
    }
}

int main()
{
    cv::Mat srcImage = cv::imread("../image/home.jpg");
    if(!srcImage.data)
    {
        std::cout << " Error of the image " << std::endl;
        return -1;
    }
    cv::namedWindow("srcImage", CV_WINDOW_NORMAL);
    cv::imshow("srcImage", srcImage);
    cv::Mat edgeXImage(srcImage.size(), srcImage.type());
    cv::Mat edgeYImage(srcImage.size(), srcImage.type());
    cv::Mat edgeImage (srcImage.size(), srcImage.type());

    std::cout << " srcImage cols: " << srcImage.cols << std::endl;

    DiffOperation(srcImage, edgeXImage, edgeYImage);
    cv::addWeighted(edgeXImage, 0.5, edgeYImage, 0.5, 0.0, edgeImage);

    cv::namedWindow("edgeXImage", CV_WINDOW_NORMAL);
    cv::namedWindow("edgeYImage", CV_WINDOW_NORMAL);
    cv::namedWindow("edgeImage", CV_WINDOW_NORMAL);
    cv::imshow("edgeXImage", edgeXImage);
    cv::imshow("edgeYImage", edgeYImage);
    cv::imshow("edgeImage", edgeImage);
    cv::waitKey(0);

    return 0;
}
