#include<ctime>
#include "EdgeDetection.h"

EdgeDetect::EdgeDetect()
{
}

EdgeDetect::~EdgeDetect()
{
}

void EdgeDetect::show()
{
    cv::imshow("srcImage", srcImage);
    cv::imshow("edgeImage", edgeImage);
    cv::waitKey(0);
}

int EdgeDetect::loadImage(char* pfile, unsigned char flag)
{
    srcImage = cv::imread(pfile, flag);
    if(srcImage.empty())
    {
        std::cout << "The srcImage is empty " << std::endl;
        return -1;
    }
    return 0;
}

// 图像差分运算求边缘
void EdgeDetect::DiffOperation()
{
    clock_t t1 = clock();
    cv::Mat edgeXImage(srcImage.size(), srcImage.type());
    cv::Mat edgeYImage(srcImage.size(), srcImage.type());
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
    cv::addWeighted(edgeXImage, 0.5, edgeYImage, 0.5, 0.0, edgeImage);
    clock_t t2 = clock();
    std::cout << " DiffOperation cost time all = " << static_cast<double>(t2-t1)*1000/CLOCKS_PER_SEC << " ms.\n";
}


// 非极大值抑制sobel的边缘检测实现：
// 将图像转换为32位浮点型 定义水平和垂直sobel算子
// 利用filter2D计算算子和图像的卷积 
// 计算卷积结果的梯度值
// 自适应计算梯度幅度值阈值 根据阈值对水平或垂直的邻域区域梯度进行比较
// 判断当前邻域梯度是否大于水平或垂直邻域梯度 自适应完成边缘检测
void EdgeDetect::sobelVerEdge()
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
    edgeImage = resultTmpMat.clone();
}


void EdgeDetect::sobelEdge(unsigned char threshold)
{
    clock_t t1 = clock();
    CV_Assert(srcImage.channels()==1);
    // 水平核因子
    cv::Mat sobelX = (cv::Mat_<double>(3,3) << 1,0,-1,2,0,-2,1,0,-1);   
    // 垂直核因子
    cv::Mat sobelY = (cv::Mat_<double>(3,3) << 1,2,1,0,0,0,-1,-2,-1);
    edgeImage = cv::Mat::zeros(srcImage.rows-2, srcImage.cols-2, srcImage.type());
    double edgeX = 0.0;
    double edgeY = 0.0;
    double graMag = 0.0;
    for(int k=1; k<srcImage.rows-1; ++k)
    for(int n=1; n<srcImage.cols-1; ++n)
    {
        edgeX = edgeY = 0.0;
        // 遍历计算水平和垂直梯度
        for(int i=-1; i<=1; ++i)
        for(int j=-1; j<=1; ++j)
        {
            edgeX += srcImage.at<uchar>(k+i,n+j)*sobelX.at<double>(i+1,j+1);
            edgeY += srcImage.at<uchar>(k+i,n+j)*sobelX.at<double>(i+1,j+1);
        }
        graMag = sqrt(edgeX*edgeX+edgeY*edgeY);
        edgeImage.at<uchar>(k-1,n-1) = ((graMag > threshold) ? 255 : 0);
    }
    clock_t t2 = clock();
    std::cout << " sobel cost all time = " << static_cast<double>(t2-t1)*1000/CLOCKS_PER_SEC << " ms. \n";
}

void EdgeDetect::sobelOptEdge(int flag)
{
    clock_t t1 = clock();
    CV_Assert(srcImage.channels()==1);
    cv::Mat sobelx = (cv::Mat_<double>(3,3) << 1, 0, -1, 2, 0, -2, 1, 0, -1);
    cv::Mat sobely = (cv::Mat_<double>(3,3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
    cv::Mat edgex, edgey;
    // 计算水平与垂直卷积
    cv::filter2D(srcImage, edgex, CV_32F, sobelx);
    cv::filter2D(srcImage, edgey, CV_32F, sobely);
    int parax = 0, paray = 0;
    switch(flag)
    {
        case 0:parax=1;paray=0;break;
        case 1:parax=0;paray=1;break;
        case 2:parax=1;paray=1;break;
        default:break;
    }
    edgex = abs(edgex); edgey = abs(edgey);
    cv::Mat graMat = parax*edgex.mul(edgex)+paray*edgey.mul(edgey);
    int scale=4;
    double thresh = scale*cv::mean(graMat).val[0];
    edgeImage = cv::Mat::zeros(srcImage.size(), srcImage.type());
    for(int i=1; i<srcImage.rows-1; ++i)
    {
        // 判断当前邻域梯度是否大于阈值与水平或垂直梯度
        float* pDataEdgeX = edgex.ptr<float>(i);
        float* pDataEdgeY = edgey.ptr<float>(i);
        float* pDataMat = graMat.ptr<float>(i);
        for(int j=1; j<srcImage.cols-1; ++j)
        {
            if(pDataMat[j]>thresh && (
              (pDataEdgeX[j]>parax*pDataEdgeY[j] && pDataMat[j]>pDataMat[j-1] && pDataMat[j]>pDataMat[j+1]) ||
              (pDataEdgeY[j]>paray*pDataEdgeX[j] && pDataMat[j]>pDataMat[j-1] && pDataMat[j]>pDataMat[j+1]) ))
                edgeImage.at<uchar>(i,j)=255;
        }
    }
    clock_t t2 = clock();
    std::cout << " sobelOpt cost time all = " << static_cast<double>(t2-t1)*1000/CLOCKS_PER_SEC << " ms. \n";
}

void EdgeDetect::laplaceEdge(unsigned char threshold)
{
    clock_t t1 = clock();
    CV_Assert(srcImage.channels()==1);
    int nRows = srcImage.rows-1;
    int nCols = srcImage.cols-1;

    edgeImage = cv::Mat::zeros(srcImage.size(), srcImage.type());
    for(int i=1; i<nRows; ++i)
    {
        int t=-1;
        uchar* pData = srcImage.ptr<uchar>(i);
        uchar* preData = srcImage.ptr<uchar>(i);
        uchar* nexData = srcImage.ptr<uchar>(i);
        for(int j=1; j<nCols; ++j)
        {
            if(pData[j] > threshold && (pData[j] > ((preData[j]+nexData[j]+pData[j-1]+pData[j+1])>>2)))
                t = 1;
            uchar tmp = srcImage.at<uchar>(i+1,j)+srcImage.at<uchar>(i-1,j)+srcImage.at<uchar>(i,j+1)+srcImage.at<uchar>(i,j-1)-4*srcImage.at<uchar>(i,j);
            if(t<=0)
                edgeImage.at<uchar>(i-1,j-1) = srcImage.at<uchar>(i,j) - tmp;
            else
                edgeImage.at<uchar>(i-1,j-1) = srcImage.at<uchar>(i,j) + tmp;
        }
    }
    clock_t t2 = clock();
    std::cout << " laplace cost time all = " << static_cast<double>(t2-t1)*1000/CLOCKS_PER_SEC << " ms. \n";
}

void EdgeDetect::robertsEdge()
{
    clock_t t1 = clock();
    CV_Assert(srcImage.channels()==1);
    int nRows = srcImage.rows-1;
    int nCols = srcImage.cols-1;
    edgeImage = cv::Mat::zeros(srcImage.size(), srcImage.type());
    for(int i=0; i<nRows; ++i)
    for(int j=0; j<nCols; ++j)
    {
        int a = srcImage.at<uchar>(i,j) - srcImage.at<uchar>(i+1,j+1);
        int b = srcImage.at<uchar>(i+1,j) - srcImage.at<uchar>(i,j+1);
        edgeImage.at<uchar>(i,j) = (uchar)sqrt(a*a+b*b);
    }
    clock_t t2 = clock();
    std::cout << " roberts cost time all = " << static_cast<double>(t2-t1)*1000/CLOCKS_PER_SEC << " ms. \n";
}

void EdgeDetect::marrHildrethEdge(int ker, double delta)
{

    clock_t t1 = clock();
    // 按公式计算LoG算子
    int kerLen = ker>>1;
    cv::Mat kernel = cv::Mat_<double>(ker,ker);
    // 滑窗
    for(int i=-kerLen; i<=kerLen; ++i)
    for(int j=-kerLen; j<=kerLen; ++j)
        // 生成内核因子
        kernel.at<double>(i+kerLen, j+kerLen) = 
        exp(-((pow(i,2)+pow(j,2))/(pow(delta,2)*2))) *
        (((pow(i,2)+pow(j,2)-2*pow(delta,2))/(2*pow(delta,4))));

    // 设置输出参数
    int kerOffset = ker>>1;
    cv::Mat laplacian = (cv::Mat_<double>(srcImage.rows-kerOffset*2,srcImage.cols-kerOffset*2));
    edgeImage = cv::Mat::zeros(srcImage.rows-kerOffset*2, srcImage.cols-kerOffset*2, srcImage.type());
    double sumLap=0.0f;
    for(int i=kerOffset; i<srcImage.rows-kerOffset; ++i)
    for(int j=kerOffset; j<srcImage.cols-kerOffset; ++j)
    {
        sumLap = 0.0f;
        // 计算图像卷积
        for(int k=-kerOffset; k<=kerOffset; ++k)
        for(int m=-kerOffset; m<=kerOffset; ++m)
            sumLap += srcImage.at<uchar>(i+k,j+m) * kernel.at<double>(kerOffset+k,kerOffset+m);
        // 生成拉普拉斯结果
        laplacian.at<double>(i-kerOffset,j-kerOffset) = sumLap;
    }
    // 过零点交叉 寻找边缘像素
    for(int i=1; i<edgeImage.rows-1; ++i)
    for(int j=1; j<edgeImage.cols-1; ++j)
    {
        edgeImage.at<uchar>(i,j) = 0;
        if( laplacian.at<double>(i+1,j  )*laplacian.at<double>(i-1,j  )<0 ||
            laplacian.at<double>(i,j+1  )*laplacian.at<double>(i,j-1  )<0 ||
            laplacian.at<double>(i+1,j-1)*laplacian.at<double>(i-1,j+1)<0 ||
            laplacian.at<double>(i-1,j-1)*laplacian.at<double>(i+1,j+1)<0  )
            edgeImage.at<uchar>(i,j) = 255;
    }
    clock_t t2 = clock();
    std::cout << " marrHildreth cost time all = " << static_cast<double>(t2-t1)*1000/CLOCKS_PER_SEC << " ms. \n ";
}
