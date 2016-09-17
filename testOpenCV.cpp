
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<iostream>

const char* file = "G:\\BaiduYunDownload\\权利的游戏S06\\0607.mp4";
// 滚动条的位置
int g_slider_position = 0;
CvCapture* g_capture = NULL;
// 滚动条回调函数
void onTrackbarSlider(int pos)
{
	cvSetCaptureProperty(
		g_capture,
		CV_CAP_PROP_POS_FRAMES,
		pos
	);
}
// 播放一个视频
void PlayVedio(const char* file)
{
	cvNamedWindow("Vedio", CV_WINDOW_AUTOSIZE);
	g_capture = cvCreateFileCapture(file);
	int frames = (int)cvGetCaptureProperty(g_capture, CV_CAP_PROP_FRAME_COUNT);
	if (frames)
		cvCreateTrackbar("Position", "Vedio", &g_slider_position, frames, onTrackbarSlider);
	IplImage* frame = NULL;
	while (1)
	{
		frame = cvQueryFrame(g_capture);
		if (!frame) break;
		cvShowImage("Vedio", frame);
		char c = cvWaitKey(33);
		if (c == 27) break;
	}
	cvReleaseCapture(&g_capture);
	cvDestroyWindow("Vedio");
}

void VedioTranslateGrayVedio(const char* file)
{
	CvCapture* capture = cvCreateFileCapture(file);
	if (!capture) return;
	IplImage* bgr_frame = cvQueryFrame(capture);
	double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	CvSize size = cvSize(
		(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
		(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT)
	);
	CvVideoWriter* writer = cvCreateVideoWriter("outPut.mp4", CV_FOURCC('M', 'J', 'P', 'G'), fps, size);
	IplImage* logpolar_frame = cvCreateImage(size, IPL_DEPTH_8U,3);
	while ((bgr_frame = cvQueryFrame(capture)) != NULL)
	{
		cvLogPolar(bgr_frame, logpolar_frame, cvPoint2D32f(bgr_frame->width / 2, bgr_frame->height / 2), 40, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
		cvWriteFrame(writer, logpolar_frame);
	}
	cvReleaseVideoWriter(&writer);
	cvReleaseImage(&logpolar_frame);
	cvReleaseCapture(&capture);
}

void onMouse(int events,int x,int y,int flags, void* param)
{
	if (events == CV_EVENT_LBUTTONDOWN && flags)
	{

	}
}

// addWeighted()实现两个图像的线性混合
void Imread()
{
	cv::Mat img = cv::imread("dota2.png");
	cv::Mat logo = cv::imread("logo.png");
	cv::namedWindow("SourceImage",CV_WINDOW_AUTOSIZE);
	cv::imshow("SourceImage", img);
	cv::imshow("logo", logo);

	// img log 混合图
	cv::Mat Roi = img(cv::Rect(750, 340,logo.cols, logo.rows));
	// dst = src1[index]*alpha + src2[index]*beta + gamma;
	cv::addWeighted(Roi, 0.5, logo, 0.1, 0.0, Roi);
	cv::imshow("img+logo", img);
	cv::waitKey(0);
}

// 多通道混合
void MultiChannelBlending()
{
	cv::Mat  logoImage = cv::imread("logo.png");
	cv::Mat  srcImage = cv::imread("dota2.png");
	if (!logoImage.data) return;
	if (!srcImage.data) return;

	cv::Mat ROI = srcImage(cv::Rect(750, 350, logoImage.cols, logoImage.rows));
	cv::addWeighted(ROI, 1.0, logoImage, 0.5, 0.0, ROI);
	cv::imshow("SourceImage", srcImage);


	std::vector<cv::Mat> channels;

	// 为了和后面的srcImage的单通道图像匹配,重新以灰度图载入图片
	logoImage = cv::imread("logo.png", 0);

	// 蓝色通道
	cv::Mat BlueImage;
	// 分离出srcImage的彩色通道
	cv::split(srcImage, channels);
	BlueImage = channels.at(0);
	ROI = BlueImage(cv::Rect(750, 350, logoImage.cols, logoImage.rows));
	cv::addWeighted(ROI, 1.0, logoImage, 0.5, 0.0, ROI);
	cv::merge(channels, srcImage);
	cv::imshow("BlueImage", srcImage);

	// 绿色通道
	srcImage = cv::imread("dota2.png");
	cv::split(srcImage, channels);
	cv::Mat GreenImage = channels.at(1);
	ROI = GreenImage(cv::Rect(750, 350, logoImage.cols, logoImage.rows));
	cv::addWeighted(ROI, 1.0, logoImage, 0.5, 0.0, ROI);
	cv::merge(channels, srcImage);
	cv::imshow("GreenImage", srcImage);

	// 红色通过
	srcImage = cv::imread("dota2.png");
	cv::split(srcImage, channels);
	cv::Mat RedImage = channels.at(2);
	ROI = RedImage(cv::Rect(750, 350, logoImage.cols, logoImage.rows));
	cv::addWeighted(ROI, 1.0f, logoImage, 0.5, 0.0f, ROI);
	cv::merge(channels, srcImage);
	cv::imshow("RedImage", srcImage);
	cv::waitKey(0);
}

cv::Mat img;
int threshval = 160;
void onTrackbar(int value, void *)
{
	cv::Mat bw = threshval < 128 ? (img < threshval) : (img > threshval);
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(bw, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	cv::Mat dst = cv::Mat::zeros(img.size(), CV_8UC3);
	if (!contours.empty() && !hierarchy.empty())
	{
		int idx = 0;
		for (; idx >= 0; idx = hierarchy[idx][0])
		{
			cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);
			drawContours(dst, contours, idx, color, CV_FILLED, 8, hierarchy);
		}
	}
	imshow("Windows", dst);
}
void TrackbarTest()
{
	img = cv::imread("girl.png",0);
	cv::createTrackbar("Threshold", "Contours", &threshval, 255, onTrackbar);
	onTrackbar(threshval, 0);
	cv::waitKey(0);
}

void ChangeChannel()
{
	cv::Mat img = cv::imread("dota2.png");
	cv::Mat logo = cv::imread("logo.png");
	cv::Mat Roi = img(cv::Rect(750, 340, logo.cols, logo.rows));
	cv::addWeighted(Roi, 0.5, logo, 0.1, 0.0, Roi);
	cv::imshow("Img+Logo", img);

	std::vector<cv::Mat> channels;
	cv::split(img, channels);
	cv::Mat blue = channels.at(0);
	cv::Mat	green = channels.at(1);
	cv::Mat red = channels.at(2);
	cv::imshow("blue", blue);
	cv::imshow("green", green);
	cv::imshow("red", red);
//	cv::addWeighted(imgRoi(cv::Rect(385, 250, logo.cols, logo.rows)), 1.0f, logo, 0.5, 0.0, imgRoi(cv::Rect(385, 250, logo.cols, logo.rows)));
	cv::waitKey(0);
}

int main()
{
	//Imread();
	//MultiChannelBlending();
	TrackbarTest();
	return 0;
}
