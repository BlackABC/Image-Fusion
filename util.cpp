#include "util.h"
#include <iostream>

void computeMinTotalVariation(cv::Mat& src, cv::Mat g, int iter, float lambda)
{
	//cv::Mat g_lambda = g / lambda;
	cv::Mat div_p = cv::Mat::zeros(src.rows, src.cols, CV_32F);
	cv::Mat div_tmp(src.rows, src.cols, CV_32F);

	for (int k = 0; k < iter; k++){

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				float p1, p2;

				//求p1
				if (i == 0)
				{
					p1 = div_p.at<float>(i, j);
				}
				else if (i == (src.rows - 1))
				{
					p1 = -div_p.at<float>(i - 1, j);
				}
				else
				{
					p1 = div_p.at<float>(i, j) - div_p.at<float>(i - 1, j);
				}
				//求p2
				if (j == 0)
				{
					p2 = div_p.at<float>(i, j);
				}
				else if (j == (src.cols - 1))
				{
					p2 = -div_p.at<float>(i, j - 1);
				}
				else
				{
					p2 = div_p.at<float>(i, j) - div_p.at<float>(i, j - 1);
				}
				div_tmp.at<float>(i, j) = p1 + p2 - g.at<uchar>(i, j) / lambda;
			}
		}

		//求delta
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				float u1, u2;
				if (i == src.rows - 1)
				{
					u1 = 0;
				}
				else{
					u1 = div_tmp.at<float>(i + 1, j) - div_tmp.at<float>(i, j);
				}

				if (j == src.cols - 1)
				{
					u2 = 0;
				}
				else{
					u2 = div_tmp.at<float>(i, j + 1) - div_tmp.at<float>(i, j);
				}

				float num = div_p.at<float>(i, j) + (u1 + u2) / 4;
				float den = 1 + abs(u1 + u2) / 4;

				div_p.at<float>(i, j) = num / den;
			}
		}
	}

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			float p1, p2;

			//求p1
			if (i == 0)
			{
				p1 = div_p.at<float>(i, j);
			}
			else if (i == (src.rows - 1))
			{
				p1 = -div_p.at<float>(i - 1, j);
			}
			else
			{
				p1 = div_p.at<float>(i, j) - div_p.at<float>(i - 1, j);
			}
			//求p2
			if (j == 0)
			{
				p2 = div_p.at<float>(i, j);
			}
			else if (j == (src.cols - 1))
			{
				p2 = -div_p.at<float>(i, j - 1);
			}
			else
			{
				p2 = div_p.at<float>(i, j) - div_p.at<float>(i, j - 1);
			}
			
			float tVal = g.at<uchar>(i, j) - lambda*(p1 + p2) + src.at<uchar>(i, j);
			tVal = std::max(0.f, std::min(tVal, 255.f));
			src.at<uchar>(i, j) = (unsigned char)tVal;
		}
	}

}