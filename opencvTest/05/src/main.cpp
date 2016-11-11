#include"EdgeDetection.h"
#include"HoughTrans.h"
#include"ShapeDetection.h"

#define sobelVer

#ifdef EDGEDETECT_TEST
int main()
{
    char* pfile = "../image/home.jpg";
    EdgeDetect edgeDetect;
    edgeDetect.loadImage(pfile, 0);
#ifdef sobel
    unsigned char threshold=125;
    edgeDetect.sobelEdge(threshold);
#endif

#ifdef diff
    edgeDetect.DiffOperation();
#endif

#ifdef sobelVer
    edgeDetect.sobelVerEdge();
#endif

#ifdef sobelOpt
    edgeDetect.sobelOptEdge(1);
#endif

#ifdef laplace
    edgeDetect.laplaceEdge(125);
#endif

#ifdef roberts
    edgeDetect.robertsEdge();
#endif

#ifdef marrhildreth
    edgeDetect.marrHildrethEdge(9,1.6);
#endif

    edgeDetect.show();
    return 0;
}
#endif


#ifdef HOUGHDETECT_TEST
int main()
{
    HoughTrans houghTrans;
    houghTrans.loadImage("../image/road.jpg", 1);

#if 0
    houghTrans.opencvHoughTransLine(1, CV_PI/180.0, 100);
#else
    houghTrans.opencvHoughTransCircle(CV_HOUGH_GRADIENT);
#endif

    houghTrans.show();

    
    return 0;
}

#endif


#ifndef SHAPEDETECT_TEST
int main()
{
    ShapeDetect shapeDetect;
    shapeDetect.loadImage("../image/tiaosan.jpg", 1);
#ifdef one
#if 1-1
    shapeDetect.opencvContours(CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, 100);
#elif 2-2
    shapeDetect.opencvConvexHull(145);
#endif
    shapeDetect.show();
#endif

#ifdef two
    shapeDetect.opencvClosingPoint();
#endif

    shapeDetect.cacBounding();
    shapeDetect.cacBoundRectAnyDirect();
    cv::waitKey(0);
    return 0;
}
#endif

