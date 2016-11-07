#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

// 非极大值抑制sobel的边缘检测实现：
// 将图像转换为32位浮点型 定义水平和垂直sobel算子
// 利用filter2D计算算子和图像的卷积 
// 计算卷积结果的梯度值
// 自适应计算梯度幅度值阈值 根据阈值对水平或垂直的邻域区域梯度进行比较
// 判断当前邻域梯度是否大于水平或垂直邻域梯度 自适应完成边缘检测
bool sobelVerEdge(cv::Mat srcImage, cv::Mat& resultImage)
{
    CV_Assert(srcImage.channels()==1);
    srcImage.convertTo(srcImage, CV_32FC1);
    // sobel算子
    cv::Mat sobelX = (cv::Mat_<float>(3,3) << -0.125, 0, 0.125, -0.25, 0, 0.25, -0.125, 0, 0.125);
    cv::Mat conMat;
    // 卷积运算
    cv::filter2D(srcImage, conMat, srcImage.type(), sobelX);
    cv::Mat graMat;
    // 计算卷积梯度幅度
    cv::multiply(conMat, conMat, graMat);
    // 根据幅度值及参数自适应计算阈值
    int scaleVal = 4;
    double thresh = scaleVal*cv::mean(graMat).val[0];
    cv::Mat resultTmpMat = cv::Mat::zeros(graMat.size(), graMat.type());
    float* pDataMag = (float*)graMat.data;
    float* pDataRes = (float*)resultTmpMat.data;
    const int nRows = conMat.rows-1;
    const int nCols = conMat.cols-1;
    for(int i=1; i<nRows; ++i)
    for(int j=1; j<nCols; ++j)
    {
        // 计算该点梯度与水平和垂直梯度值的大小比较
        bool b1 = (pDataMag[i*nCols+j]>pDataMag[i*nCols+j-1]);
        bool b2 = (pDataMag[i*nCols+j]>pDataMag[i*nCols+j+1]);
        bool b3 = (pDataMag[i*nCols+j]>pDataMag[(i-1)*nCols+j]);
        bool b4 = (pDataMag[i*nCols+j]>pDataMag[(i+1)*nCols+j]);
        // 判断邻域梯度是否满足大于水平或垂直梯度的条件
        pDataRes[i*nCols+j] = 255*((pDataMag[i*nCols+j]>thresh) && ((b1&&b2)||(b3&&b4)));
    }
    resultTmpMat.convertTo(resultTmpMat, CV_8UC1);
    resultImage = resultTmpMat.clone();
    return true;
}

int main()
{
    cv::Mat srcImage = cv::imread("../image/home.jpg", 0);
    if(srcImage.empty())
    {
        std::cout << " Empty Image " << std::endl;
        return -1;
    }
    cv::imshow("srcImage", srcImage);
    cv::Mat sobelImage;
    sobelVerEdge(srcImage, sobelImage);
    cv::imshow("sobelImage", sobelImage);
    cv::waitKey(0);
    return 0;
}
