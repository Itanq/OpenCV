#include"EdgeDetection.h"

#define sobelVer

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

#ifdef Houg
int main()
{
    cv::Mat srcImage = cv::imread("../image/home.jpg", 0);
    if(srcImage.empty())
    {
        std::cout << " the image is error " << std::endl;
        return -1;
    }
    cv::Mat edgeMat, houghMat;
    cv::canny(srcImage, edgeMat, 50, 200, 3);
    cv::cvtColor(edgeMat, houghMat, CV_GRAY2BGR);
#if 0
    std::vector<cv::Vec2f> lines;

#endif
}

#endif
