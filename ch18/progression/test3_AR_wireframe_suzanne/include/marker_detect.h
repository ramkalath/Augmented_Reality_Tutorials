//												Amme Sharanam!!!

#ifndef MARKER_DETECT_H
#define MARKER_DETECT_H

class marker
{
	private:
	int size;
	cv::Mat calib_frame;
	double ThreshParam1, ThreshParam2;
	float probDetect,  markersize;
	aruco::BoardConfiguration boardconfig;
	aruco::BoardDetector boarddetect;
	aruco::CameraParameters camparams;
	vector< cv::Point3f > object_points;
	vector< cv::Point2f > image_points;

public:
	cv::Mat  rvecs, tvecs;
	bool detect_flag;
	marker(char**, cv::Mat&, cv::Mat&);
	void marker_detect(cv::Mat);
}; 

#endif