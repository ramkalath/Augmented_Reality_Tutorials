/*****************************************************************************
 * Author : Ramkumar
 * Date : 02/June/2018
 * Email : ramkalath@gmail.com
 * Breif Description : overlay of cube on the marker. Refactored the code a bit and made it clean. It will be easier to use in the forthcoming exercises
 * Detailed Description : To compile type 'make' and to run type 'run.sh'
 *****************************************************************************/

// all the imports
#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <aruco/aruco.h> // include the aruco import
#include <strings.h>
#include <vector>
#include "marker_detect.h"

int main(int argc, char **argv)
{
	if(argc < 3) // checks for argument list
	{
		cerr<<"Not enough arguments.\nUsage is : ./binary videodevice_number boardConfig.yml intrinsics.yml [size] [out]"<<endl;
		return 0;
	}

	cv::Mat intrinsic_matrix;					
	cv::Mat distortion_parameters;
	cv::Mat extrinsics;

	marker marker1(argv, intrinsic_matrix, distortion_parameters); // marker object is created and arguments are passed to the constructor

	cv::Mat frame;
	cv::VideoCapture cap(atoi(argv[1]));

	while(1)
	{
		cap >> frame;
		marker1.marker_detect(frame);

		// there is a detect_flag variable which is of bool datatype and is set to 1 if the marker is detected.
		if(marker1.detect_flag)
		{
			cv::Rodrigues(marker1.rvecs, extrinsics);
			std::cout << extrinsics << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
