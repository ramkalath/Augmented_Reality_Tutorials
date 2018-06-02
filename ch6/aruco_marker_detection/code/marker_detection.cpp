/*****************************************************************************
 * Author : Ramkumar
 * Date : 02/June/2018
 * Email : ramkalath@gmail.com
 * Breif Description : demo of marker detection with aruco
 * Detailed Description : To compile type 'make' and to run type 'run.sh'
 *****************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <aruco/aruco.h>
#include <strings.h>
#include <vector>

int main(int argc, char **argv)
{
	int cam_device_no, key, size, row, column;
	double ThreshParam1, ThreshParam2;
	float probDetect,  markersize;

	cv::Mat frame;
	aruco::BoardConfiguration boardconfig;
	aruco::CameraParameters camparams;
	aruco::BoardDetector boarddetect;
	
	ThreshParam1 = 10;
	ThreshParam2 = 10;
	
	if(argc < 3)
	{
		cerr<<"Not enough arguments.\nUsage is : ./binary videodevice_number BoardConfig.yml CameraConfig.yml [size]"<<endl;
		return 0;
	}

	cam_device_no = atoi(argv[1]);
	cv::VideoCapture cap(cam_device_no);
	boardconfig.readFromFile(argv[2]);
	cap >> frame;
	camparams.readFromXMLFile(argv[3]);
	camparams.resize(frame.size());
	markersize = atoi(argv[4]);
	boarddetect.setParams(boardconfig, camparams, markersize);
	boarddetect.getMarkerDetector().getThresholdParams(ThreshParam1, ThreshParam2);

	while(1)
	{
		cap >> frame;
		probDetect = boarddetect.detect(frame); 	// gives the strength of detection
		size = boarddetect.getDetectedMarkers().size();
		for (int i=0;i<size;i++)
			boarddetect.getDetectedMarkers()[i].draw(frame, cv::Scalar(0, 0, 255), 1);

		cv::imshow("", frame);
		key = cv::waitKey(10);
		if(key==27)
			break;
		else if(key==115)
			cv::imwrite("marker_with_data.jpg", frame);
	}
	return 0;
}
