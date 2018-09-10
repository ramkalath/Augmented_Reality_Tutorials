//											Amme Sharanam!!!

#ifndef CUBE_H
#define CUBE_H

class cube
{
private:
	vector< cv::Point3f > cube_obj_pts;
	vector< cv::Point2f > cube_img_pts;

public:
	cube();
	cv::Mat drawcube(cv::Mat, cv::Mat, cv::Mat, cv::Mat, cv::Mat);
};

#endif