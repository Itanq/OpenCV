
#include "LicensePlateNumber.h"

CarLicenseDetect::CarLicenseDetect()
{
}

CarLicenseDetect::~CarLicenseDetect()
{
}

void CarLicenseDetect::show() const
{
    if(carSrcImage.data) cv::imshow("carSrcImage", carSrcImage);
    if(ansImage.data) cv::imshow("LicensePlateNumber", ansImage);
    cv::waitKey(0);
}

int CarLicenseDetect::loadImage(char* pfile, int flag)
{
    carSrcImage = cv::imread(pfile, flag);
    if(carSrcImage.empty())
    {
        std::cout << " the image is empty " << std::endl;
        return -1;
    }
    return 0;
}

void CarLicenseDetect::RGB2HSV()
{
    std::vector<cv::Mat> hsvImage;

    cv::cvtColor(carSrcImage, carHSVImage, CV_BGR2HSV);
    cv::split(carHSVImage, hsvImage); 

    img_h = hsvImage[0];
    img_s = hsvImage[1];
    img_v = hsvImage[2];

    img_h.convertTo(img_h, CV_32F);
    img_s.convertTo(img_s, CV_32F);
    img_v.convertTo(img_v, CV_32F);

    double max_h=0, max_s=0, max_v=0;
    cv::minMaxIdx(img_h, 0, &max_h);
    cv::minMaxIdx(img_s, 0, &max_s);
    cv::minMaxIdx(img_v, 0, &max_v);

    img_h /= max_h; img_s /= max_s; img_v /= max_v;
    cv::cvtColor(carSrcImage, grayImage, CV_BGR2GRAY);
}


void CarLicenseDetect::sobelVerEdge()
{
    CV_Assert(grayImage.channels()==1);
    grayImage.convertTo(grayImage, CV_32FC1);
    cv::Mat sobelx = (cv::Mat_<float>(3,3)<<-0.125,0,0.125,-0.25,0,0.25,-0.125,0,0.125);
    cv::Mat ConResMat;
    cv::filter2D(grayImage, ConResMat, grayImage.type(), sobelx);
    cv::Mat graMagMat;
    cv::multiply(ConResMat, ConResMat, graMagMat);
    int scaleVal = 4;
    double thresh = scaleVal*cv::mean(graMagMat).val[0];
    edgeImage = cv::Mat::zeros(graMagMat.size(), graMagMat.type());
    float* pdataMag = (float*)graMagMat.data;
    float* pdataRes = (float*)edgeImage.data;
    const int nRows = ConResMat.rows;
    const int nCols = ConResMat.cols;
    for(int i=1; i<nRows-1; ++i)
    for(int j=1; j<nCols-1; ++j)
    {
        bool b1 = pdataMag[i*nCols+j]>pdataMag[i*nCols+j-1];
        bool b2 = pdataMag[i*nCols+j]>pdataMag[i*nCols+j+1];
        bool b3 = pdataMag[i*nCols+j]>pdataMag[(i-1)*nCols+j];
        bool b4 = pdataMag[i*nCols+j]>pdataMag[(i+1)*nCols+j];
        pdataRes[i*nCols+j] = 255 * ( (pdataMag[i*nCols+j]>thresh) && ((b1&&b2)||(b3&&b4)) );
    }
    edgeImage.convertTo(edgeImage, CV_8UC1);
}

void CarLicenseDetect::suspectedAreaExtract(cv::Mat& bw_edge)
{
    cv::Mat bw = ((img_h>0.45)&(img_h<0.75) & (img_s>0.75) & (img_v>0.25));
    int height = bw.rows, width = bw.cols;
    bw_edge = cv::Mat::zeros(bw.size(), bw.type());
    for(int i=1; i<height-2; ++i)
    for(int j=1; j<width-2; ++j)
    {
        cv::Rect rect;
        rect.x = j-1;rect.x=i-1;
        rect.width = rect.height = 3;
        if( (edgeImage.at<uchar>(i,j)==255) && (cv::countNonZero(bw(rect))>=1) )
            bw_edge.at<uchar>(i,j) = 255;
    }
}


void CarLicenseDetect::carLicensePlateExtract()
{
    RGB2HSV();
    sobelVerEdge();
    cv::Mat bw_edge;
    suspectedAreaExtract(bw_edge);
    
    cv::Mat morph;
    cv::morphologyEx(bw_edge, morph, cv::MORPH_CLOSE, cv::Mat::zeros(2,25,CV_8UC1));
    cv::Mat tmp;
    cv::resize(bw_edge, tmp, cv::Size(), 1, 1);
    std::vector<std::vector<cv::Point>> region_contours;
    cv::findContours(morph.clone(), region_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));
    std::vector<cv::Rect> candidates;
    std::vector<cv::Mat> candidates_img;
    cv::Mat result;
    cv::imshow("morph", morph);
    cv::waitKey(0);
    std::cout << "region_contours.size()="<<region_contours.size()<<std::endl;
    for(size_t i=0; i!=region_contours.size(); ++i)
    {
        cv::Rect rect = cv::boundingRect(region_contours[i]);
        int sub = cv::countNonZero(morph(rect));
        double ratio = (double)sub/rect.area();
        double wh_ratio = (double)rect.width/rect.height;
//        if(ratio>0.5 && wh_ratio>2 && wh_ratio<5 && rect.height>12 && rect.width>60)
        {
            cv::Mat small = bw_edge(rect);
            ansImage = carSrcImage(rect);
            cv::imshow("rect", ansImage);
        }
    }
}

