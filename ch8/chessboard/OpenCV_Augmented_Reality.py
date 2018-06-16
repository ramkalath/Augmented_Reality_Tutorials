# *****************************************************************************
# * Filename : extrinsic_matrix_calibration.py
# * Date : 11/June/2018
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : Implements AR with OpenCV chessboard
# * Detailed Description : to run this code type './run.sh'. Find the chessboard image in ch6/chessboard_detection/chessboard_image.png
# *****************************************************************************

import numpy as np
import cv2
import yaml
import time

criteria = ( cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_COUNT, 30, 0.1 )

if __name__ == "__main__":
    cap = cv2.VideoCapture(0)

    pattern_size = (9, 6)

    col0 = np.tile(np.arange(0, pattern_size[0]), pattern_size[1])
    col1 = np.reshape((np.ones((pattern_size[0], pattern_size[1]))* np.arange(pattern_size[1])).T, (1, pattern_size[0]*pattern_size[1] ))[0]
    col2 = np.zeros(pattern_size[1]*pattern_size[0])
    pattern_points = np.float32(np.vstack([col0, col1, col2]).T.reshape(pattern_size[1]*pattern_size[0], 3)) # a small addendum, converted float64 into float32

    stream = open("./camera_parameters.yml", "r") # read the intrinsic matrix from the camera_parameters.yml file
    stream_info = yaml.load(stream)
    intrinsic_matrix = np.array(stream_info["camera_matrix"]["data"]).reshape(3, 3) # we obtain the camera matrix 
    dist_coefs = np.array(stream_info["distortion_coefficients"]["data"]) # we also obtain the distortion coefficients

    axis = np.float32([[0,0,0], [3,0,0], [0,3,0], [0,0,-3]]) # the points to be drawn are define here as axis points
    axis1 = np.concatenate((axis, np.array([[1],[1],[1],[1]])), axis=1) # they are then converted into homogeneous coordinates

    while True:
        flag, frame = cap.read()
        found, corners = cv2.findChessboardCorners(frame, pattern_size) # corners of the chessboard are then detected. We dont find cornerSubPix() because its an expensive operation and number of positive hits are much less compared to the gross findChessboardCorners() method. Also its an overkill
        if(found):
            rvecs, tvecs, inliers = cv2.solvePnPRansac(pattern_points, corners, intrinsic_matrix, dist_coefs) # we use solvePnP() method to calculate the extrinsics -> rvecs and tvecs
            rotation_matrix = cv2.Rodrigues(rvecs)[0] # then we convert the quaternions rvecs into the rotation matrix
            extrinsic_matrix =  np.concatenate((rotation_matrix, tvecs), axis = 1) # tvecs can be appended onto the rotation matrix
            camera_matrix = np.dot(intrinsic_matrix, extrinsic_matrix) # finally we compute the camera_matrix by combining the intrinsic and extrinsic matrix
            homog_coord = np.dot(axis1, camera_matrix.T).T # Then we calculate the projection of 3D points on the image plane of the camera. Output is in homogeneous coordinate system
            x = homog_coord[0]/homog_coord[-1] # x /
            y = homog_coord[1]/homog_coord[-1] # and y carries the coordinates for the four points of the axes

            # we then draw those ---------------------------------------------------------
            cv2.line(frame, (int(x[0]), int(y[0])), (int(x[1]), int(y[1])), (255,0,0), 5)
            cv2.line(frame, (int(x[0]), int(y[0])), (int(x[2]), int(y[2])), (0,255,0), 5)
            cv2.line(frame, (int(x[0]), int(y[0])), (int(x[3]), int(y[3])), (0,0,255), 5)

        # and finally display
        cv2.imshow("AR_frame", frame)
        key = cv2.waitKey(20)
        if(key==27):
            break
        if(key==115):
            cv2.imwrite("AR_frame.jpg", frame)
    cv2.destroyAllWindows()
