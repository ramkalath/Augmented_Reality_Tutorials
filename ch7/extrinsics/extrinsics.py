# *****************************************************************************
# * Filename : extrinsic_matrix_calibration.py
# * Date : 11/June/2018
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : Implements calculation of extrinsic matrix with opencv
# * Detailed Description : to run this code type './run.sh'. Find the chessboard image in ch6/chessboard_detection/chessboard_image.png
# *****************************************************************************

import numpy as np
import cv2
import yaml

criteria = ( cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_COUNT, 30, 0.1 )

if __name__ == "__main__":
    cap = cv2.VideoCapture(0)

    pattern_size = (9, 6)

    col0 = np.tile(np.arange(0, pattern_size[0]), pattern_size[1])
    col1 = np.reshape((np.ones((pattern_size[0], pattern_size[1]))* np.arange(pattern_size[1])).T, (1, pattern_size[0]*pattern_size[1] ))[0]
    col2 = np.zeros(pattern_size[1]*pattern_size[0])
    object_points = np.float32(np.vstack([col0, col1, col2]).T.reshape(pattern_size[1]*pattern_size[0], 3)) # a small addendum, converted float64 into float32

    stream = open("./cameraConfig.yml", "r") # read the intrinsic matrix from the camera_parameters.yml file
    stream_info = yaml.load(stream)
    intrinsic_matrix = np.array(stream_info["camera_matrix"]["data"]).reshape(3, 3) # we obtain the camera matrix 
    dist_coefs = np.array(stream_info["distortion_coefficients"]["data"]) # we also obtain the distortion coefficients

    while True:
        flag, frame = cap.read()
        found, corners = cv2.findChessboardCorners(frame, pattern_size) # corners of the chessboard are then detected. We dont find cornerSubPix() because its an expensive operation and number of positive hits are much less compared to the gross findChessboardCorners() method. Also its an overkill
        if(found):
            rvecs, tvecs, inliers = cv2.solvePnPRansac(object_points, corners, intrinsic_matrix, dist_coefs) # we use solvePnP() method to calculate the extrinsics -> rvecs and tvecs
            rotation_matrix = cv2.Rodrigues(rvecs)[0] # then we convert the quaternions rvecs into the rotation matrix
            extrinsic_matrix =  np.concatenate((rotation_matrix, tvecs), axis = 1) # tvecs can be appended onto the rotation matrix

            print "extrinsic_matrix = ", extrinsic_matrix # print the extrinsic matrix

        cv2.imshow("extrinsic", frame)
        key = cv2.waitKey(20)
        if(key==27):
            break
    cv2.destroyAllWindows()
