#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <aruco/aruco.h> // include the aruco import
#include <strings.h>
#include <vector>
#include "marker_detect.h"

using namespace cv;
using namespace std;

//*********************************************************************************************************************************************************************
// draws a cube on top of the marker given the frame, intrinsic_matrix, distortion_matrix, rvecs and tvecs
// there is a constructor
// there is a cv::Mat drawcube(cv::Mat frame, cv::Mat intrinsic_matrix. cv::Mat distortion_matrix, cv::Mat rvecs, cv::Mat rvecs) which returns the frame with the cube overlayed on it

class cube
{
private:
	vector< cv::Point3f > cube_obj_pts;
	vector< cv::Point2f > cube_img_pts;

public:
	cube();
	cv::Mat drawcube(cv::Mat, cv::Mat, cv::Mat, cv::Mat, cv::Mat);
};

cube::cube()
{
	//*********Define cube object points*******************************
	Point3f c1(0, 0, 0),
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
}

cv::Mat cube::drawcube(cv::Mat frame, cv::Mat intrinsic_matrix, cv::Mat distortion_parameters, cv::Mat rvecs, cv::Mat tvecs)
{

	cv::projectPoints(cube_obj_pts, rvecs, tvecs, intrinsic_matrix, distortion_parameters, cube_img_pts);

	for(int i=0; i< cube_img_pts.size(); i++)
		circle(frame, cube_img_pts[i], 2, Scalar( 0, 255, 0 ), 2, 8 );
	line(frame, cube_img_pts[0], cube_img_pts[1], Scalar(0,0,255), 2, CV_AA);
	line(frame, cube_img_pts[1], cube_img_pts[2], Scalar(0,0,255), 2, CV_AA);
	line(frame, cube_img_pts[2], cube_img_pts[3], Scalar(0,0,255), 2, CV_AA);
	line(frame, cube_img_pts[3], cube_img_pts[0], Scalar(0,0,255), 2, CV_AA);
	line(frame, cube_img_pts[0], cube_img_pts[4], Scalar(0,0,255), 2, CV_AA);
	line(frame, cube_img_pts[1], cube_img_pts[5], Scalar(0,0,255), 2, CV_AA); 
	line(frame, cube_img_pts[2], cube_img_pts[6], Scalar(0,0,255), 2, CV_AA); 
	line(frame, cube_img_pts[3], cube_img_pts[7], Scalar(0,0,255), 2, CV_AA);
	line(frame, cube_img_pts[4], cube_img_pts[5], Scalar(0,0,255), 2, CV_AA);
	line(frame, cube_img_pts[5], cube_img_pts[6], Scalar(0,0,255), 2, CV_AA);
	line(frame, cube_img_pts[6], cube_img_pts[7], Scalar(0,0,255), 2, CV_AA);
	line(frame, cube_img_pts[7], cube_img_pts[4], Scalar(0,0,255), 2, CV_AA);

	return frame;
}
//**********************************************************************************************************************************************************************
 
