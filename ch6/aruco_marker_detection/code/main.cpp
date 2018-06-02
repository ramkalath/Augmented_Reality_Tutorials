/*****************************************************************************
 * Author : Ram
 * Date : <date>
 * Email : ramkalath@gmail.com
 * Breif Description :
 * Detailed Description :
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
	std::vector<cv::Point3f> object_points;
	std::vector<cv::Point2f> image_points;
	std::vector<cv::Point3f> cube_obj_pts;
	std::vector<cv::Point2f> cube_img_pts;
	
	cv::Mat rvecs(3, 1, cv::DataType<double>::type);
	cv::Mat tvecs(3, 1, cv::DataType<double>::type);

	ThreshParam1 = 10;
	ThreshParam2 = 10;
	
	if(argc < 3)
	{
		cerr<<"Not enough arguments.\nUsage is : ./binary videodevice_number          boardConfig.yml intrinsics.yml [size] [out]"<<endl;
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
	//***Define cube object points *************************************************
	cv::Point3f c1(0, 0, 0),
			c2(11, 0, 0),
			c3(11, 11, 0),
			c4(0, 11, 0),
			c5(0, 0, -11),
			c6(11, 0, -11),
			c7(11, 11, -11),
			c8(0, 11, -11);
	cube_obj_pts.push_back(c1);
	cube_obj_pts.push_back(c2);
	cube_obj_pts.push_back(c3);
	cube_obj_pts.push_back(c4);
	cube_obj_pts.push_back(c5);
	cube_obj_pts.push_back(c6);
	cube_obj_pts.push_back(c7);
	cube_obj_pts.push_back(c8);

	std::cout << boardconfig.getIdList();

	while(1)
	{
		//cap >> frame;
		frame = cv::imread("webcam_snapshot.jpg");
		probDetect = boarddetect.detect(frame);
		size = boarddetect.getDetectedMarkers().size();
		object_points.clear();
		image_points.clear();
		std::cout << boarddetect.getDetectedMarkers()[0] << std::endl;
		//for (int i=0;i<size;i++)
		//{
			//boarddetect.getDetectedMarkers()[i].draw(frame, cv::Scalar(0,0,255),1);
			//std::cout << boarddetect.getDetectedMarkers()[i].x
		//}

		cv::imshow("", frame);
		key = cv::waitKey(10);
		if(key==27)
			break;
		else if(key==115)
			cv::imwrite("marker_with_data.jpg", frame);
	}
	return 0;
}
