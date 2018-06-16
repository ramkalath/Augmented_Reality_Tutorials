/*****************************************************************************
 * Author : Ramkumar
 * Date : 02/June/2018
 * Email : ramkalath@gmail.com
 * Breif Description : demo of marker detection with aruco
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

int main(int argc, char **argv)
{
	// some variables we will be using later
	int cam_device_no, size;
	double ThreshParam1 = 10, ThreshParam2 = 10;
	float probDetect,  markersize;
	
	cv::Mat frame; // Mat is the data structure that holds an image in opencv-cpp
	aruco::BoardConfiguration boardconfig; 
	aruco::CameraParameters camparams;
	aruco::BoardDetector boarddetect;
	
	if(argc < 3)
	{
		std::cerr << "Not enough arguments.\nUsage is : ./binary videodevice_number BoardConfig.yml CameraConfig.yml [size]" << std::endl;
		return 0;
	}

	// all the settings that aruco's datastructures/constructors need -----------------------
	cam_device_no = atoi(argv[1]);
	cv::VideoCapture cap(cam_device_no);
	boardconfig.readFromFile(argv[2]);
	cap >> frame;
	camparams.readFromXMLFile(argv[3]);
	camparams.resize(frame.size());
	markersize = atoi(argv[4]);
	boarddetect.setParams(boardconfig, camparams, markersize);
	boarddetect.getMarkerDetector().getThresholdParams(ThreshParam1, ThreshParam2);
	// --------------------------------------------------------------------------------------

	while(1)
	{
		cap >> frame;
		probDetect = boarddetect.detect(frame); 	// gives the strength of detection
		size = boarddetect.getDetectedMarkers().size(); // amongst the 10 markers on the board, only some might be detected at a particular time

		for (int i=0;i<size;i++)
			boarddetect.getDetectedMarkers()[i].draw(frame, cv::Scalar(0, 0, 255), 1); // For each detection draw the boundaries of the marker and the marker id

		cv::imshow("", frame);
		int key = cv::waitKey(10);
		if(key==27)
			break;
		else if(key==115)
			cv::imwrite("marker_with_data.jpg", frame);
	}
	return 0;
}
