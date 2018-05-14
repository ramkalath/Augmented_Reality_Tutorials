/*****************************************************************************
 * Author : Ram
 * Date : 14/May/2018
 * Email : ramkalath@gmail.com
 * Breif Description : Testing OpenCV installation
 * Detailed Description : to compile type 'make' and to run './test_opencv'
 *****************************************************************************/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main()
{
	cv::Mat img;
	img = cv::imread("test.jpg");
	cv::imshow("my first opencv window", img);
	cv::waitKey();
	return 0;
}
