#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include "util.h"

int main()
{
	//test RGB to YUV
	cv::Mat infrared = cv::imread("Vlasakkers_IR.tif", 0);
	cv::Mat visible_origin = cv::imread("Vlasakkers_Vis.tif");

	//这里要保证两幅图像已经配准，并图像大小一致
	cv::Mat visible;
	cv::resize(visible_origin, visible, infrared.size());
	
	cv::imshow("infrared", infrared);
	cv::imshow("visible", visible);

	//将可见光图像由RGB空间转换到YUV空间
	cv::Mat yuv;
	cv::cvtColor(visible, yuv, CV_BGR2YUV);
	std::vector<cv::Mat> yuvArr;
	cv::split(yuv, yuvArr);
	
	//YUV空间中的Y空间显示了图像的亮度，与红外图像进行融合
	//主要的融合步骤
	cv::Mat g = infrared - yuvArr[0];

	//统计时间
	double start = cv::getTickCount();

	//计算全变分，并将图像融合，融合后返回给YUV图像的Y层
	computeMinTotalVariation(yuvArr[0], g, 30, 0.4);

	//统计时间
	double end = cv::getTickCount();

	//全变分最小化算法的运行时间
	double tt = (end - start) / cv::getTickFrequency();
	std::cout << tt << std::endl;

	//融合成一幅三通道图像
	cv::Mat fusionImage;
	cv::merge(yuvArr, fusionImage);

	//将图像从YUV空间转换到RGB空间，显示图像
	cv::cvtColor(fusionImage, fusionImage, CV_YUV2BGR);
	cv::imshow("fusionImage", fusionImage);
	
	cv::waitKey(0);
	return 0;
}