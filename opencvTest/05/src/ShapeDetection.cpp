
#include "ShapeDetection.h"

ShapeDetect::ShapeDetect()
{
}

ShapeDetect::~ShapeDetect()
{
}

void ShapeDetect::show()
{
    if(srcImage.data) cv::imshow("srcImage", srcImage);
    if(edgeImage.data) cv::imshow("edgeImage", edgeImage);
    if(contourImage.data) cv::imshow("contourImage", contourImage);
    if(hullImage.data) cv::imshow("hullImage", hullImage);
    cv::waitKey(0);
}

int ShapeDetect::loadImage(char* pfile, int flag)
{
    srcImage = cv::imread(pfile, flag);
    if(srcImage.empty())
    {
        std::cout << " the image is empty " << std::endl;
        return -1;
    }
    return 0;
}


void ShapeDetect::opencvContours(int mode, int method, int thresh)
{
    cv::Mat grayImage;
    cv::cvtColor(srcImage, grayImage, CV_BGR2GRAY);
    cv::blur(grayImage, grayImage, cv::Size(3,3));
    cv::Canny(grayImage, edgeImage, thresh, thresh*2, 3);

    cv::findContours(edgeImage, contours, hierarchy, mode, method, cv::Point(0,0));

    contourImage = cv::Mat::zeros(edgeImage.size(), CV_8UC3);
    for(int i=0; i<contours.size(); ++i)
    {
        cv::Scalar color = cv::Scalar(rand()%255, rand()%255, rand()%255);
        drawContours(contourImage, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
    }
}

void ShapeDetect::opencvConvexHull(int thresh)
{
    cv::cvtColor(srcImage, grayImage, CV_BGR2GRAY);
    cv::blur(grayImage, grayImage, cv::Size(3,3));

    cv::Mat edgeImage;
    // 阈值化操作
    cv::threshold(grayImage, edgeImage, thresh, 255, cv::THRESH_BINARY);
    // 轮廓检测
    findContours(edgeImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));
    // 凸包及缺陷检测参数
    std::vector<std::vector<int>> intHull(contours.size());
    std::vector<std::vector<cv::Point>> pointHull(contours.size());
    std::vector<std::vector<cv::Vec4i>> hullDefect(contours.size());
    for(size_t i=0; i<contours.size(); ++i)
    {
        // point 类型凸包检测
        cv::convexHull(cv::Mat(contours[i]), pointHull[i], false);
        // int 类型凸包检测
        cv::convexHull(cv::Mat(contours[i]), intHull[i], false);
        // 凸包缺陷检测
        cv::convexityDefects(cv::Mat(contours[i]), intHull[i], hullDefect[i]);
    }
    hullImage = cv::Mat::zeros(edgeImage.size(), CV_8UC3);
    for(size_t i=0; i<contours.size(); ++i)
    {
        cv::Scalar color = cv::Scalar(rand()%255, rand()%255, rand()%255);
        cv::drawContours(hullImage, contours, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
        cv::drawContours(hullImage, pointHull,i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());

        size_t cnt = contours.size();
        if(cnt < 300) continue;
        std::vector<cv::Vec4i>::iterator iterDefects = hullDefect[i].begin();
        while(iterDefects!=hullDefect[i].end())
        {
            cv::Vec4i& v = (*iterDefects);
            cv::Point ptStart(contours[i][v[0]]);
            cv::Point ptEnd(contours[i][v[1]]);
            cv::Point ptFar(contours[i][v[2]]);
            int depth = v[3]/256;
            if(depth > 20 && depth < 80)
            {
                cv::line(hullImage, ptStart, ptFar, CV_RGB(0,255,0),2);
                cv::line(hullImage, ptEnd, ptFar, CV_RGB(0,255,0),2);
                cv::circle(hullImage, ptEnd, 4, cv::Scalar(255,0,100),2);
                cv::circle(hullImage, ptStart, 4, cv::Scalar(255,0,100),2);
                cv::circle(hullImage, ptFar, 4, cv::Scalar(100,0,255),2);
            }
            ++iterDefects;
        }
    }
}

#if 0
void ShapeDetect::opencvClosingPoint()
{
    srcImage = cv::Mat(500,500,CV_8UC3);
    cv::RNG rng = cv::theRNG();
    while(1)   
    {
        int cnt = rng.uniform(1,101);
        std::vector<cv::Point> points;
        // 随机生成点集
        for(int i=0; i<cnt; ++i)
        {
            cv::Point pt;
            pt.x = rng.uniform(srcImage.rows/4, srcImage.cols*3/4);
            pt.y = rng.uniform(srcImage.rows/4, srcImage.cols*3/4);
            points.push_back(pt);
        }
        srcImage = cv::Scalar::all(0);
        for(int i=0; i<cnt; ++i)
            cv::circle(srcImage, points[i], 3, cv::Scalar(0,0,255),CV_FILLED,CV_AA);

        // 计算点集形成的最小矩形
        cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
        cv::Point2f center, vtx[4];
        float radius=0;
        // 计算点集形成的最小圆
        cv::minEnclosingCircle(cv::Mat(points), center, radius);

        // 绘制最小矩形
        box.points(vtx); // 最小矩形的四个点
        for(int i=0; i<4; ++i)
            cv::line(srcImage, vtx[i], vtx[(i+1)%4], cv::Scalar(0,255,0),1,CV_AA);
        // 绘制最小圆
        cv::circle(srcImage, center, cvRound(radius),cv::Scalar(0,255,255),1,CV_AA);

        cv::imshow("rect&circle",srcImage);

        char key=(char)cv::waitKey();
        if(key==27||key=='q')break;
    }
}
#endif

void ShapeDetect::cvt_threh()
{
    cv::cvtColor(srcImage, grayImage, CV_BGR2GRAY);
    cv::blur(grayImage, grayImage, cv::Size(3,3));
    // 阈值化
    cv::threshold(grayImage, edgeImage, 120, 255, cv::THRESH_BINARY);
    // 计算边界轮廓
    cv::findContours(edgeImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));

}

void ShapeDetect::cacBounding()
{
    cv::RNG rng(12345);
    // 转换,阈值化,查找轮廓
    cvt_threh();
    std::vector<std::vector<cv::Point>> conPoint(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());
    std::vector<cv::Point2f> center(contours.size());
    std::vector<float> radius(contours.size());
    for(int i=0; i<contours.size(); ++i)
    {
        // 多边形近似 - 轮廓
        cv::approxPolyDP(cv::Mat(contours[i]), conPoint[i], 3, true);
        // 计算最小外界矩形框
        boundRect[i] = cv::boundingRect(cv::Mat(conPoint[i]));
        // 计算最小封闭圆
        cv::minEnclosingCircle(conPoint[i], center[i], radius[i]);
    }
    // 绘制
    cv::Mat resImage = cv::Mat::zeros(edgeImage.size(), CV_8UC3);
    for(int i=0; i<contours.size(); ++i)
    {
        cv::Scalar color = cv::Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        cv::drawContours(resImage, conPoint, i,color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
        cv::rectangle(resImage, boundRect[i], color, 2, 8, 0);
        cv::circle(resImage, center[i], radius[i], color, 2, 8, 0);
    }
    cv::imshow("resImage-BoundRect", resImage);
}

void ShapeDetect::cacBoundRectAnyDirect()
{
    cv::RNG rng(12345);
    cvt_threh();
    std::vector<cv::RotatedRect> minRect(contours.size());
    std::vector<cv::RotatedRect> minEllipse(contours.size());
    for(int i=0; i<contours.size(); ++i)
    {
        // 计算最小面积框
        minRect[i] = cv::minAreaRect(cv::Mat(contours[i]));
        // 计算外接轮廓的椭圆
        if(contours[i].size()>5)
            minEllipse[i] = cv::fitEllipse(cv::Mat(contours[i]));
    }
    // 绘制
    cv::Mat resImage = cv::Mat::zeros(edgeImage.size(), CV_8UC3);
    for(int i=0; i<contours.size(); ++i)
    {
        cv::Scalar color = cv::Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        cv::drawContours(resImage, contours, i, color, 2, 8);
        cv::ellipse(resImage, minEllipse[i], color, 2, 8);
        cv::Point2f fpoint[4];
        minRect[i].points(fpoint);
        for(int i=0; i<4; ++i)
            cv::line(resImage, fpoint[i], fpoint[(i+1)%4], color, 2, 8, 0);
    }
    cv::imshow("resImage-BoundRectAnyDirect", resImage);
}

