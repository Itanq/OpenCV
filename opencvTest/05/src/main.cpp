#include"EdgeDetection.h"
#include"HoughTrans.h"

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


#define HOUGHDETECT_TEST
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
