#include <cmath>
#include "HoughTrans.h"
#define PI 3.1415926


HoughTrans::HoughTrans()
{
}

HoughTrans::~HoughTrans()
{
}


int HoughTrans::loadImage(char* pfile, int flag)
{
    srcImage = cv::imread(pfile, flag);
    if(srcImage.empty())
    {
        std::cout << " the image is empty " << std::endl;
        return -1;
    }
    return 0;
}

void HoughTrans::show()
{
    if(srcImage.data)
        cv::imshow("srcImage", srcImage);
    if(edgeImage.data)
        cv::imshow("edgeImage", edgeImage);
    if(houghImage.data)
        cv::imshow("houghImage", houghImage);
    cv::waitKey(0);
}

#if 0
void HoughTrans::HoughLineTrans(int houghSpace, int threshold)
{
    CV_Assert(srcImage.channels()==1);

    int centerX = srcImage.cols>>1;   
    int centerY = srcImage.rows>>1;
    double houghInterval = PI/(double)houghSpace;
    
    int max = Max(srcImage.cols, srcImage.rows);
    int max_len = (int)(std::sqrt(2.0)*max);
    int * hough1D = new int[2*houghSpace*max_len];
    int **hough2D = new int[houghSpace][2*max_len]();
    
    for(int i=0; i<srcImage.rows; ++i)
    for(int j=0; j<srcImage.cols; ++j)
    {
        int p = srcImage.at<uchar>(i,j) & 0xff;
        if(p==0) continue;
        for(int cell=0; cell<houghSpace; ++cell)
        {
            max = (int)((j-centerX)*std::cos(cell*houghInterval)+(i-centerY)*std::sin(cell*houghInterval));
            max += max_len;
            if(max<0 || max>=2*max_len) continue;
            hough2D[cell][max] += 1;
        }
    }

    int max_hough = 0;
    for(int i=0; i<houghSpace; ++i)
    for(int j=0; j<2*max_len; ++j)
    {
        hough1D[(i+j*houghSpace)] = hough2D[i][j];
        max_hough = Max(max_hough, hough2D[i][j]);
    }

    int hough_threshold = (int)(threshold*max_hough);
    for(int i=0; i<houghSpace; ++i)
    for(int j=0; j<2*max_len; ++j)
    {
        if(hough2D[i][j]<hough_threshold) continue;
        int hough_value = hough2D[i][j];
        bool isLine=true;
        for(int ii=-1; ii<=1; ++ii)
        for(int jj=-1; jj<=1; ++jj) if(ii||jj)
        {
            int yf = i+ii, xf=j+jj;
            if(xf<0) continue;
            if(xf<2*max_len)
            {
                if(yf<0) yf += houghSpace;
                if(yf>=houghSpace) yf -= houghSpace;
                if(hough2D[yf][xf]<=hough_value) continue;
                isLine=false;
                break;
            }
        }
        if(!isLine) continue;
        
        double dy = std::sin(i*houghInterval);
        double dx = std::cos(i*houghInterval);
        if(i<=houghSpace/4||i>=3*houghSpace/4)
        {
            for(int ii=0; ii<srcImage.rows; ++ii)
            {
                int subcol = (int)((j-max_len-((ii-centerY)*dy))/dx)+centerX;

            }
        }
    }
}
#endif

void HoughTrans::opencvHoughTransLine(double rho, double theta, int threshold)
{
    cv::Canny(srcImage, edgeImage, 50, 200, 3);
    cv::cvtColor(edgeImage, houghImage, CV_GRAY2BGR);
#if 0
    std::vector<cv::Vec2f> lines;
    // 标准霍夫变换,检测二值图像中直线
    cv::HoughLines(edgeImage, lines, rho, theta, threshold);
    // 根据直线参数表达式绘制结果
    for(size_t i=0; i<lines.size(); ++i)
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = std::cos(theta), b = std::sin(theta);
        double x0=a*rho, y0=b*rho;
        pt1.x = cvRound(x0+1000*(-b));
        pt1.y = cvRound(y0+1000*  a );
        pt2.x = cvRound(x0-1000*(-b));
        pt2.y = cvRound(y0-1000*  a );
        cv::line(houghImage, pt1, pt2, cv::Scalar(0,0,255),3,CV_AA);
    }
#else
    std::vector<cv::Vec4i> lines;
    // 统计概率霍夫变换
    cv::HoughLinesP(edgeImage, lines, 1, CV_PI/180.0, 50, 50, 10);
    for(size_t i=0; i<lines.size(); ++i)
    {
        cv::Vec4i l = lines[i];
        cv::line(houghImage, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(0,0,255), 3, CV_AA);
    }
#endif
}


void HoughTrans::opencvHoughTransCircle(int method)
{
    cv::Mat grayImage;
    houghImage = srcImage.clone();
    cv::Canny(srcImage, grayImage, 50, 200, 3);
    cv::GaussianBlur(grayImage, grayImage, cv::Size(9,9), 2, 2);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(grayImage, circles, method, 1, srcImage.rows/8, 200, 100, 0, 0);
    for(size_t i=0; i<circles.size(); ++i)
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cv::circle(houghImage, center, 5, cv::Scalar(0,0,255), -1, 8, 0);
        cv::circle(houghImage, center, radius, cv::Scalar(255,0,0), 10, 8, 0);
    }
}



