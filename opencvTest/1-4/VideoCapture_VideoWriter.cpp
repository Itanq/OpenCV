
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<iostream>

int TestVideoCapture()
{
	cv::VideoCapture capture("G:\\BaiduYunDownload\\权利的游戏S06\\0605.mp4");
	if (!capture.isOpened()) return -1;
	// get 方法获取 VideoCapture 的各种属性
	long nTotalFrame = capture.get(CV_CAP_PROP_FRAME_COUNT);
	std::cout << "Total Frame : " << nTotalFrame << std::endl;

	int frameHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	int frameWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	std::cout << "Frame Height: " << frameHeight << std::endl;
	std::cout << "Frame Width: " << frameWidth << std::endl;

	double frameRate = capture.get(CV_CAP_PROP_FPS);
	std::cout << "FrameRate: " << frameRate << std::endl;

	cv::Mat frameImage;
	while (1)
	{
		capture >> frameImage;
		if (!frameImage.empty()) cv::imshow("frameImage", frameImage);
		else break;
		if (char(cv::waitKey(frameRate)) == 'q') break;
	}
	capture.release();
	return 0;
}

int TestVideoWrite()
{
	const char* inputVideoPath = "G:\\BaiduYunDownload\\权利的游戏S06\\0605.mp4";
	char* outputVedioPath = "testVideoWrite.avi";

	// 输入的视频
	cv::VideoCapture inputVideo(inputVideoPath);
	if (!inputVideo.isOpened()) return -1;
	double fps = inputVideo.get(CV_CAP_PROP_FPS);

	// 输出的视频
	cv::VideoWriter outputVideo;
	cv::Size videoSize = cv::Size((int)inputVideo.get(CV_CAP_PROP_FRAME_WIDTH), (int)inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));

	outputVideo.open(outputVedioPath, -1, fps, videoSize, true);
	if (!outputVideo.isOpened()) return -1;

	cv::Mat frameImg;
	cv::Mat resultImg;
	std::vector<cv::Mat> rgb;
	while (true)
	{
		inputVideo >> frameImg;
		if (!frameImg.empty())
		{
			cv::imshow("frameImg", frameImg);
			// 分离出三通道
			cv::split(frameImg, rgb);
			for (int i = 0; i < 3; ++i)
			{
				// 提取 B 通道
				if (i != 0)
					rgb[i] = cv::Mat::zeros(videoSize, rgb[0].type());
				// 通道合并
				cv::merge(rgb, resultImg);
			}
			cv::imshow("outPut", resultImg);
			outputVideo << resultImg;
		}
		else break;
		if(char(cv::waitKey(fps))=='q') break;
	}

}
int main()
{
//	TestVideoCapture();
	TestVideoWrite();
	return 0;
}