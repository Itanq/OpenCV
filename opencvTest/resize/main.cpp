#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>


int main()
{
    cv::Mat srcImage = cv::imread("../image/car.jpg");
    cv::Rect rect;
    rect.x = 270; rect.y = 720; rect.width=250; rect.height=100;

    cv::Rect extra_roi;
    
    float cx = rect.x + rect.width / 2;
    float cy = rect.y + rect.height / 2;

    float _scale_w = 1.8;
    float _scale_h = 1.8;

    extra_roi.width = rect.width *  _scale_w;
    extra_roi.height = rect.height * _scale_h;
    extra_roi.x = cx - extra_roi.width / 2;
    extra_roi.y = cy - extra_roi.height / 2;
    std::cout << cv::format(" the start Image size %d %d ", srcImage.cols, srcImage.rows) << std::endl;
    std::cout << cv::format(" the rect is %d %d %d %d ", rect.x, rect.y, rect.width, rect.height) << std::endl;
    std::cout << cv::format(" the padding rect is %d %d %d %d ", extra_roi.x, extra_roi.y, extra_roi.width, extra_roi.height) << std::endl;

    cv::rectangle(srcImage, rect, cv::Scalar(0,255,0),2);
    cv::rectangle(srcImage, extra_roi, cv::Scalar(255,0,0),2);
    cv::namedWindow("srcImage",CV_WINDOW_KEEPRATIO);
    cv::imshow("srcImage", srcImage);

    cv::resize(srcImage, srcImage, cv::Size(srcImage.cols*0.5,srcImage.rows*0.5));
    
    cv::imshow("reImage", srcImage);


    cv::waitKey(0);
    return 0;
}

