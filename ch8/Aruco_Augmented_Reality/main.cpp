/*****************************************************************************
 * Author : Ramkumar
 * Date : 13/June/2018
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
#include "cube.h"

int main(int argc, char **argv)
{
	if(argc < 3) // checks for argument list
	{
		cerr<<"Not enough arguments.\nUsage is : ./binary videodevice_number boardConfig.yml intrinsics.yml [size] [out]"<<endl;
		return 0;
	}

	cv::Mat intrinsic_matrix;					
	cv::Mat distortion_parameters;

	marker marker1(argv, intrinsic_matrix, distortion_parameters); // marker object is created and arguments are passed to the constructor
	cube cube1; // This is the object we have been drawing

	cv::Mat frame;
	cv::VideoCapture cap(atoi(argv[1]));

	while(1)
	{
		cap >> frame;
		marker1.marker_detect(frame);

		// there is a detect_flag variable which is of bool datatype and is set to 1 if the marker is detected.
		if(marker1.detect_flag)
			frame = cube1.drawcube(frame, intrinsic_matrix, distortion_parameters, marker1.rvecs, marker1.tvecs); // Draws the cube
		cv::imshow("cubeoverlayed", frame);
		int key = cv::waitKey(10);
		if(key == 27) break;
		if(key == 115)
		{
			cv::imwrite("cubeoverlayed.jpg", frame);
			break;
		}
	}

	return EXIT_SUCCESS;
}
