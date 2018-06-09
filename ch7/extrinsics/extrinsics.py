# *****************************************************************************
# * Filename : extrinsic_matrix_calibration.py
# * Date : 9/June/2018
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : Implements calculation of extrinsic matrix with opencv
# * Detailed Description : to run this code type './run.sh'. Find the chessboard image in ch6/chessboard_detection/chessboard_image.png
# *****************************************************************************
import cv2
import numpy as np
import yaml

pattern_size = (9, 6) # number of corners in the chessboard

if __name__ == "__main__":
    with open('./camera_parameters.yml', 'r') as f:
        data = yaml.load(f)
        intrinsic_matrix = np.array(data["camera_matrix"]["data"]).reshape(3, 3)
        distortion_coeffs = np.array(data["distortion_coefficients"]["data"])

    # let us define object points
    # col0 = np.tile(np.arange(0, pattern_size[0]), pattern_size[1])
    # col1 = np.reshape((np.ones((pattern_size[0], pattern_size[1]))* np.arange(pattern_size[1])).T, (1, pattern_size[0]*pattern_size[1] ))[0]
    # col2 = np.zeros(pattern_size[1]*pattern_size[0])
    # object_points = np.float32(np.vstack([col0, col1, col2]).T.reshape(pattern_size[1]*pattern_size[0], 3)) # a small addendum, converted float64 into float32
    objp = np.zeros((6*7,3), np.float32)
    objp[:,:2] = np.mgrid[0:7,0:6].T.reshape(-1,2)

    cap = cv2.VideoCapture(0)
    while True:
        flag, frame = cap.read()
        found, corners = cv2.findChessboardCorners(frame, pattern_size)
        ret, rvecs, tvecs, inliers = cv2.solvePnP(objp, corners, intrinsic_matrix, distortion_coeffs)
        cv2.imshow("", frame)
        key = cv2.waitKey(10)
        if(key == 27):
            break
