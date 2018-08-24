#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include "util.h"

int main()
{
	//test RGB to YUV
	cv::Mat infrared = cv::imread("Vlasakkers_IR.tif", 0);
	cv::Mat visible_origin = cv::imread("Vlasakkers_Vis.tif");

	//����Ҫ��֤����ͼ���Ѿ���׼����ͼ���Сһ��
	cv::Mat visible;
	cv::resize(visible_origin, visible, infrared.size());
	
	cv::imshow("infrared", infrared);
	cv::imshow("visible", visible);

	//���ɼ���ͼ����RGB�ռ�ת����YUV�ռ�
	cv::Mat yuv;
	cv::cvtColor(visible, yuv, CV_BGR2YUV);
	std::vector<cv::Mat> yuvArr;
	cv::split(yuv, yuvArr);
	
	//YUV�ռ��е�Y�ռ���ʾ��ͼ������ȣ������ͼ������ں�
	//��Ҫ���ںϲ���
	cv::Mat g = infrared - yuvArr[0];

	//ͳ��ʱ��
	double start = cv::getTickCount();

	//����ȫ��֣�����ͼ���ںϣ��ںϺ󷵻ظ�YUVͼ���Y��
	computeMinTotalVariation(yuvArr[0], g, 30, 0.4);

	//ͳ��ʱ��
	double end = cv::getTickCount();

	//ȫ�����С���㷨������ʱ��
	double tt = (end - start) / cv::getTickFrequency();
	std::cout << tt << std::endl;

	//�ںϳ�һ����ͨ��ͼ��
	cv::Mat fusionImage;
	cv::merge(yuvArr, fusionImage);

	//��ͼ���YUV�ռ�ת����RGB�ռ䣬��ʾͼ��
	cv::cvtColor(fusionImage, fusionImage, CV_YUV2BGR);
	cv::imshow("fusionImage", fusionImage);
	
	cv::waitKey(0);
	return 0;
}