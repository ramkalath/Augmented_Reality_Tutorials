/*****************************************************************************
 * Author : Ram
 * Date : 14/May/2018
 * Email : ramkalath@gmail.com
 * Breif Description : implements aruco marker detection
 * Detailed Description : I was too lazy to parse out the marker id's and automatically generate the object points and image points. I hardcoded it :P
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <aruco/aruco.h> // include the aruco import
#include <strings.h>
#include <vector>

using namespace cv;
using namespace std;

//********************************************************************
// creates a marker class with a constructor initializing several variables
// to detect a marker the constructor has a prototype marker(char** args, cv::Mat& intrinsic_matrix, cv::Mat& distortion_coefficients);
// args has the format ()
// the next two cv::Mat& and cv::Mat& takes in the pointer for intrinsic_matrix and distortion_coefficients. Aruco reads the intrinsic_matrix and the distortion_coefficients from an xml file. 
// the marker_detect(cv::Mat frame) takes in a webcam grabbed frame for marker detection. 
// rvecs and tvecs are declared under public to be accessed in main for passing to the object overlay method.

class marker
{
private:
	int size;
	cv::Mat calib_frame;
	double ThreshParam1, ThreshParam2;
	float probDetect, markersize;
	aruco::BoardConfiguration boardconfig;
	aruco::BoardDetector boarddetect;
	aruco::CameraParameters camparams;
	vector<cv::Point3f> object_points;
	vector<cv::Point2f> image_points;

public:
	cv::Mat  rvecs, tvecs;
	bool detect_flag;
	marker(char**, cv::Mat&, cv::Mat&);
	void marker_detect(cv::Mat);
};

// constructor 
marker::marker(char** argv, cv::Mat& intrinsic_matrix, cv::Mat& distortion_parameters)
{
	boardconfig.readFromFile(argv[2]);
	ThreshParam1 = 10;
	ThreshParam2 = 10;
	camparams.readFromXMLFile(argv[3]);
	cv::VideoCapture cap1(atoi(argv[1]));
	cap1 >> calib_frame;
	camparams.resize(calib_frame.size());
	markersize = atoi(argv[4]);
	boarddetect.setParams(boardconfig, camparams, markersize);
	boarddetect.getMarkerDetector().getThresholdParams(ThreshParam1, ThreshParam2);
	cv::Mat rvecs(3,1,DataType<double>::type);
	cv::Mat tvecs(3,1,DataType<double>::type);
	intrinsic_matrix = camparams.CameraMatrix;
	distortion_parameters = camparams.Distorsion;	
	detect_flag = 0;
}

// marker detection function which detects markers and gives out rvecs and tvecs
// there are a set of image and object points which aruco detects. The image points are detected and the dumped into a stack. Corresponding object points are dumped into a stack as well. 
// finally tvecs and rvecs are calibrated.
void marker::marker_detect(cv::Mat frame)
{
		probDetect = boarddetect.detect(frame);
		size = 0;
		detect_flag = 0;
		size = boarddetect.getDetectedMarkers().size();
		object_points.clear();
		image_points.clear();
	
		for (int i=0;i<size;i++)
		{	
			//boarddetect.getDetectedMarkers()[i].draw(frame,Scalar(0,0,255),1);	
			switch(boarddetect.getDetectedMarkers()[i].id)	
			{		
  				case 547:
  				{	
 					Point2f one_img_pt(boarddetect.getDetectedMarkers()[i][0].x, boarddetect.getDetectedMarkers()[i][0].y),
 							two_img_pt(boarddetect.getDetectedMarkers()[i][1].x, boarddetect.getDetectedMarkers()[i][1].y), 
							three_img_pt(boarddetect.getDetectedMarkers()[i][2].x, boarddetect.getDetectedMarkers()[i][2].y), 
							four_img_pt(boarddetect.getDetectedMarkers()[i][3].x, boarddetect.getDetectedMarkers()[i][3].y);
							
					Point3f one_obj_pt(5, 5, 0),
							two_obj_pt(0, 5, 0),
							three_obj_pt(0, 0, 0),
							four_obj_pt(5, 0, 0);
					
					image_points.push_back(one_img_pt);
					image_points.push_back(two_img_pt);
					image_points.push_back(three_img_pt);
					image_points.push_back(four_img_pt);
					
					object_points.push_back(one_obj_pt);
					object_points.push_back(two_obj_pt);
					object_points.push_back(three_obj_pt);
					object_points.push_back(four_obj_pt);
					//cout<<object_points<<"\n"<<image_points<<"\n\n\n\n";
					break;
 				}

 				case 182:
 				{
					Point2f one_img_pt(boarddetect.getDetectedMarkers()[i][0].x, boarddetect.getDetectedMarkers()[i][0].y),
							two_img_pt(boarddetect.getDetectedMarkers()[i][1].x, boarddetect.getDetectedMarkers()[i][1].y), 
							three_img_pt(boarddetect.getDetectedMarkers()[i][2].x, boarddetect.getDetectedMarkers()[i][2].y), 
							four_img_pt(boarddetect.getDetectedMarkers()[i][3].x, boarddetect.getDetectedMarkers()[i][3].y);
							
					Point3f one_obj_pt(11, 5, 0),
							two_obj_pt(6, 5, 0),
							three_obj_pt(6, 0, 0),
							four_obj_pt(11, 0, 0);
					
					image_points.push_back(one_img_pt);
					image_points.push_back(two_img_pt);
					image_points.push_back(three_img_pt);
					image_points.push_back(four_img_pt);
					
					object_points.push_back(one_obj_pt);
					object_points.push_back(two_obj_pt);
					object_points.push_back(three_obj_pt);
					object_points.push_back(four_obj_pt);
					//cout<<object_points<<"\n"<<image_points<<"\n\n\n\n";
					break;
 				}

				case 939:
				{
					Point2f one_img_pt(boarddetect.getDetectedMarkers()[i][0].x, boarddetect.getDetectedMarkers()[i][0].y),
							two_img_pt(boarddetect.getDetectedMarkers()[i][1].x, boarddetect.getDetectedMarkers()[i][1].y), 
							three_img_pt(boarddetect.getDetectedMarkers()[i][2].x, boarddetect.getDetectedMarkers()[i][2].y), 
							four_img_pt(boarddetect.getDetectedMarkers()[i][3].x, boarddetect.getDetectedMarkers()[i][3].y);
							
					Point3f one_obj_pt(17, 5, 0),
							two_obj_pt(12, 5, 0),
							three_obj_pt(12, 0, 0),
							four_obj_pt(17, 0, 0);
					
					image_points.push_back(one_img_pt);
					image_points.push_back(two_img_pt);
					image_points.push_back(three_img_pt);
					image_points.push_back(four_img_pt);
					
					object_points.push_back(one_obj_pt);
					object_points.push_back(two_obj_pt);
					object_points.push_back(three_obj_pt);
					object_points.push_back(four_obj_pt);
					//cout<<object_points<<"\n"<<image_points<<"\n\n\n\n";
					break;
					
				}

				case 1008:
				{
					Point2f one_img_pt(boarddetect.getDetectedMarkers()[i][0].x, boarddetect.getDetectedMarkers()[i][0].y),
							two_img_pt(boarddetect.getDetectedMarkers()[i][1].x, boarddetect.getDetectedMarkers()[i][1].y), 
							three_img_pt(boarddetect.getDetectedMarkers()[i][2].x, boarddetect.getDetectedMarkers()[i][2].y), 
							four_img_pt(boarddetect.getDetectedMarkers()[i][3].x, boarddetect.getDetectedMarkers()[i][3].y);
							
					Point3f one_obj_pt(23, 5, 0),
							two_obj_pt(18, 5, 0),
							three_obj_pt(18, 0, 0),
							four_obj_pt(23, 0, 0);
					
					image_points.push_back(one_img_pt);
					image_points.push_back(two_img_pt);
					image_points.push_back(three_img_pt);
					image_points.push_back(four_img_pt);
					
					object_points.push_back(one_obj_pt);
					object_points.push_back(two_obj_pt);
					object_points.push_back(three_obj_pt);
					object_points.push_back(four_obj_pt);
					//cout<<object_points<<"\n"<<image_points<<"\n\n\n\n";
					break;
				}

				case 537:
				{
					Point2f one_img_pt(boarddetect.getDetectedMarkers()[i][0].x, boarddetect.getDetectedMarkers()[i][0].y),
							two_img_pt(boarddetect.getDetectedMarkers()[i][1].x, boarddetect.getDetectedMarkers()[i][1].y), 
							three_img_pt(boarddetect.getDetectedMarkers()[i][2].x, boarddetect.getDetectedMarkers()[i][2].y), 
							four_img_pt(boarddetect.getDetectedMarkers()[i][3].x, boarddetect.getDetectedMarkers()[i][3].y);
							
					Point3f one_obj_pt(29, 5, 0),
							two_obj_pt(24, 5, 0),
							three_obj_pt(24, 0, 0),
							four_obj_pt(29, 0, 0);
					
					image_points.push_back(one_img_pt);
					image_points.push_back(two_img_pt);
					image_points.push_back(three_img_pt);
					image_points.push_back(four_img_pt);
					
					object_points.push_back(one_obj_pt);
					object_points.push_back(two_obj_pt);
					object_points.push_back(three_obj_pt);
					object_points.push_back(four_obj_pt);
					//cout<<object_points<<"\n"<<image_points<<"\n\n\n\n";
					break;
				}

				case 517:
 				{
					Point2f one_img_pt(boarddetect.getDetectedMarkers()[i][0].x, boarddetect.getDetectedMarkers()[i][0].y),
							two_img_pt(boarddetect.getDetectedMarkers()[i][1].x, boarddetect.getDetectedMarkers()[i][1].y), 
							three_img_pt(boarddetect.getDetectedMarkers()[i][2].x, boarddetect.getDetectedMarkers()[i][2].y), 
							four_img_pt(boarddetect.getDetectedMarkers()[i][3].x, boarddetect.getDetectedMarkers()[i][3].y);
							
					Point3f one_obj_pt(5, 11, 0),
							two_obj_pt(0, 11, 0),
							three_obj_pt(0, 6, 0),
							four_obj_pt(5, 6, 0);
					
					image_points.push_back(one_img_pt);
					image_points.push_back(two_img_pt);
					image_points.push_back(three_img_pt);
					image_points.push_back(four_img_pt);
					
					object_points.push_back(one_obj_pt);
					object_points.push_back(two_obj_pt);
					object_points.push_back(three_obj_pt);
					object_points.push_back(four_obj_pt);
					//cout<<object_points<<"\n"<<image_points<<"\n\n\n\n";
					break;
 				}
						
				
				case 861:
				{
					Point2f one_img_pt(boarddetect.getDetectedMarkers()[i][0].x, boarddetect.getDetectedMarkers()[i][0].y),
							two_img_pt(boarddetect.getDetectedMarkers()[i][1].x, boarddetect.getDetectedMarkers()[i][1].y), 
							three_img_pt(boarddetect.getDetectedMarkers()[i][2].x, boarddetect.getDetectedMarkers()[i][2].y), 
							four_img_pt(boarddetect.getDetectedMarkers()[i][3].x, boarddetect.getDetectedMarkers()[i][3].y);
							
					Point3f one_obj_pt(11, 11, 0),
							two_obj_pt(6, 11, 0),
							three_obj_pt(6, 6, 0),
							four_obj_pt(11, 6, 0);
					
					image_points.push_back(one_img_pt);
					image_points.push_back(two_img_pt);
					image_points.push_back(three_img_pt);
					image_points.push_back(four_img_pt);
					
					object_points.push_back(one_obj_pt);
					object_points.push_back(two_obj_pt);
					object_points.push_back(three_obj_pt);
					object_points.push_back(four_obj_pt);
					//cout<<object_points<<"\n"<<image_points<<"\n\n\n\n";
					break;
				}
				
				
				case 631:
				{
					Point2f one_img_pt(boarddetect.getDetectedMarkers()[i][0].x, boarddetect.getDetectedMarkers()[i][0].y),
							two_img_pt(boarddetect.getDetectedMarkers()[i][1].x, boarddetect.getDetectedMarkers()[i][1].y), 
							three_img_pt(boarddetect.getDetectedMarkers()[i][2].x, boarddetect.getDetectedMarkers()[i][2].y), 
							four_img_pt(boarddetect.getDetectedMarkers()[i][3].x, boarddetect.getDetectedMarkers()[i][3].y);
							
					Point3f one_obj_pt(17, 11, 0),
							two_obj_pt(12, 11, 0),
							three_obj_pt(12, 6, 0),
							four_obj_pt(17, 6, 0);
					
					image_points.push_back(one_img_pt);
					image_points.push_back(two_img_pt);
					image_points.push_back(three_img_pt);
					image_points.push_back(four_img_pt);
					
					object_points.push_back(one_obj_pt);
					object_points.push_back(two_obj_pt);
					object_points.push_back(three_obj_pt);
					object_points.push_back(four_obj_pt);
					//cout<<object_points<<"\n"<<image_points<<"\n\n\n\n";
					break;
				}
				
				case 227:
				{	
					Point2f one_img_pt(boarddetect.getDetectedMarkers()[i][0].x, boarddetect.getDetectedMarkers()[i][0].y),
							two_img_pt(boarddetect.getDetectedMarkers()[i][1].x, boarddetect.getDetectedMarkers()[i][1].y), 
							three_img_pt(boarddetect.getDetectedMarkers()[i][2].x, boarddetect.getDetectedMarkers()[i][2].y), 
							four_img_pt(boarddetect.getDetectedMarkers()[i][3].x, boarddetect.getDetectedMarkers()[i][3].y);
							
					Point3f one_obj_pt(23, 11, 0),
							two_obj_pt(18, 11, 0),
							three_obj_pt(18, 6, 0),
							four_obj_pt(23, 6, 0);
					
					image_points.push_back(one_img_pt);
					image_points.push_back(two_img_pt);
					image_points.push_back(three_img_pt);
					image_points.push_back(four_img_pt);
					
					object_points.push_back(one_obj_pt);
					object_points.push_back(two_obj_pt);
					object_points.push_back(three_obj_pt);
					object_points.push_back(four_obj_pt);
					//cout<<object_points<<"\n"<<image_points<<"\n\n\n\n";
					break;
				}
				
				
				case 942:
				{
					Point2f one_img_pt(boarddetect.getDetectedMarkers()[i][0].x, boarddetect.getDetectedMarkers()[i][0].y),
							two_img_pt(boarddetect.getDetectedMarkers()[i][1].x, boarddetect.getDetectedMarkers()[i][1].y), 
							three_img_pt(boarddetect.getDetectedMarkers()[i][2].x, boarddetect.getDetectedMarkers()[i][2].y), 
							four_img_pt(boarddetect.getDetectedMarkers()[i][3].x, boarddetect.getDetectedMarkers()[i][3].y);
							
					Point3f one_obj_pt(29, 11, 0),
							two_obj_pt(24, 11, 0),
							three_obj_pt(24, 6, 0),
							four_obj_pt(29, 6, 0);
					
					image_points.push_back(one_img_pt);
					image_points.push_back(two_img_pt);
					image_points.push_back(three_img_pt);
					image_points.push_back(four_img_pt);
					
					object_points.push_back(one_obj_pt);
					object_points.push_back(two_obj_pt);
					object_points.push_back(three_obj_pt);
					object_points.push_back(four_obj_pt);
					//cout<<object_points<<"\n"<<image_points<<"\n\n\n\n";
					break;
				}
			}			
		}

		if (size > 1)
			detect_flag = 1;
		
		if(detect_flag)
			cv::solvePnP(object_points, image_points, camparams.CameraMatrix, camparams.Distorsion, rvecs, tvecs);
}
 
