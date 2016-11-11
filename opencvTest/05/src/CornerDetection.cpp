
#include"CornerDetection.h"

CornerDetect::CornerDetect()
{
}

CornerDetect::~CornerDetect()
{
}

int CornerDetect::loadImage(char* pfile, int flag)
{
    srcImage = cv::imread(pfile, flag);
    if(!srcImage.data)
    {
        std::cout << " the image is erorr " << std::endl;
        return -1;
    }
    return 0;
}

void CornerDetect::show()
{
    if(srcImage.data) cv::imshow("srcImage",srcImage);
    if(grayImage.data) cv::imshow("grayImage",grayImage);
    if(edgeImage.data) cv::imshow("edgeImage",edgeImage);
    if(cornerImage.data) cv::imshow("cornerImage",cornerImage);
    cv::waitKey(0);
}

int ppow(uchar a){ return (int)a*a; }
int mmin(int a,int b){ return a<b?a:b; }

void CornerDetect::cacMoravec(int size, int thresh)
{
    cv::cvtColor(srcImage, grayImage, CV_BGR2GRAY);
    cv::blur(grayImage, grayImage, cv::Size(3,3));
    cornerImage = srcImage.clone();
    int k = size >> 1;
    int nRows = cornerImage.rows;
    int nCols = cornerImage.cols;
    std::vector<cv::Point> points;

    // 滑窗遍历寻找兴趣点
    for(int i=k; i<nRows-k; ++i)
    for(int j=k; j<nCols-k; ++j)
    {
        int v1=0, v2=0, v3=0, v4=0;
        for(int c=-k; c<k; ++c) // 水平
            v1 += ppow(grayImage.at<uchar>(i,j+c)-grayImage.at<uchar>(i,j+c+1));
        for(int r=-k; r<k; ++r) // 垂直
            v2 += ppow(grayImage.at<uchar>(i+r,j)-grayImage.at<uchar>(i+r+1,j));
        for(int a=-k; a<k; ++a) // 45'
            v3 += ppow(grayImage.at<uchar>(i+a,j+a)-grayImage.at<uchar>(i+a+1,j+a+1));
        for(int a=-k; a<k; ++a) // 135'
            v4 += ppow(grayImage.at<uchar>(i+a,j-a)-grayImage.at<uchar>(i+a+1,j-a-1));
        // 取最小值作为最终兴趣点
        int val = mmin(v1,mmin(v2,mmin(v3,v4)));
        if(val>thresh) // 该兴趣点大于阈值则存入点集
            points.push_back(cv::Point(j,i));
    }
    for(int i=0; i<points.size(); ++i)
        cv::circle(cornerImage, points[i], 3, cv::Scalar(0,0,255));
}

void CornerDetect::cacHarris(int blockSize, int kerSize, double k)
{
    cv::cvtColor(srcImage, grayImage, CV_BGR2GRAY);
    cornerImage = cv::Mat::zeros(srcImage.size(), CV_32FC1);
    int depth = srcImage.depth();
    double scale = (double)(1<< ((kerSize>0?kerSize:3)-1))*blockSize;
    if(depth==CV_8U) scale *= 255;
    scale = 1/scale;
    // Sobel 滤波
    cv::Mat dx, dy;
    cv::Sobel(grayImage, dx, CV_32F, 1, 0, kerSize, scale, 0);
    cv::Sobel(grayImage, dy, CV_32F, 0, 1, kerSize, scale, 0);

    // 求解水平和垂直梯度
    cv::Size size = srcImage.size();
    cv::Mat cov(size, CV_32FC3);
    for(int i=0; i<size.height; ++i)
    {
        float* covData = (float*)(cov.data+i*cov.step);
        const float *dxData = (const float*)(dx.data+i*dx.step);
        const float *dyData = (const float*)(dy.data+i*dy.step);
        for(int j=0; j<size.width; ++j)
        {
            float dx_ = dxData[j];
            float dy_ = dyData[j];
            covData[3*j] = dx_*dy_;
            covData[3*j+1] = dx_*dy_;
            covData[3*j+2] = dx_*dy_;
        }
    }
    // 盒滤波操作
    cv::boxFilter(cov, cov, cov.depth(), cv::Size(blockSize, blockSize));

    if(cov.isContinuous() && cornerImage.isContinuous())
    {
        size.width *= size.height;
        size.height = 1;
    }
    else
        size = cornerImage.size();
    // 计算响应函数
    for(int i=0; i<size.height; ++i)
    {
        float* pData = (float*)(cornerImage.data+i*cornerImage.step);
        const float* covData = (const float*)(cov.data+i*cov.step);
        for(int j=0; j<size.width; ++j)
        {
            float a = covData[3*j];
            float b = covData[3*j+1];
            float c = covData[3*j+2];
            pData[j] = a*c-b*b-k*(a+c)*(a+c);
        }
    }
    // 归一化
    cv::normalize(cornerImage,cornerImage,0,255,cv::NORM_MINMAX,CV_32FC1,cv::Mat());
    cv::convertScaleAbs(cornerImage,cornerImage);
    // 绘制角点
    cv::Mat src;
    srcImage.copyTo(src);
    for(int i=0; i<cornerImage.rows; ++i)
    for(int j=0; j<cornerImage.cols; ++j)
        if((int)cornerImage.at<float>(i,j)>150)
            cv::circle(src, cv::Point(i,j), 5, cv::Scalar(0,0,255),2,8,0);
    src.copyTo(cornerImage);
}


void CornerDetect::cacShi_Tomasi()
{
    cv::cvtColor(srcImage, grayImage, CV_BGR2GRAY);
    std::vector<cv::Point2f> vecCorners;
    double quality = 0.01;
    double minDist = 10;
    int blockSize = 3;
    int maxNumCor=50;
    cv::goodFeaturesToTrack(grayImage, vecCorners, maxNumCor, quality, minDist, cv::Mat(), blockSize, false, 0.04);
    for(int i=0; i<vecCorners.size(); ++i)
        cv::circle(srcImage, vecCorners[i], 4, cv::Scalar(0,0,255),2);
}


