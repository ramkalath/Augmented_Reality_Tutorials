# *****************************************************************************
# * Filename : intrinsic_matrix_calibration.py
# * Date : 7/June/2018
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : implements camera calibration with opencv
# * Detailed Description : to run this code, type './run.sh'. Find the chessboard image in ch6/chessboard_detection/chessboard_image.png
# *****************************************************************************

import numpy as np
import cv2

criteria = ( cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_COUNT, 30, 0.1 )

if __name__ == "__main__":
    cap = cv2.VideoCapture(0)

    pattern_size = (9, 6) # number of corners in the chessboard

    # we need to describe some points that we are going to track
    # we need something like [[0, 0, 0], [1, 0, 0] . . . .  [cols-1, 0, 0], [0, 1, 0], [1, 1, 0] . . . . [cols-1, rows-1, 0]]
    # we compile each col separately and stack them together
    col0 = np.tile(np.arange(0, pattern_size[0]), pattern_size[1])
    col1 = np.reshape((np.ones((pattern_size[0], pattern_size[1]))* np.arange(pattern_size[1])).T, (1, pattern_size[0]*pattern_size[1] ))[0]
    col2 = np.zeros(pattern_size[1]*pattern_size[0])
    pattern_points = np.float32(np.vstack([col0, col1, col2]).T.reshape(pattern_size[1]*pattern_size[0], 3)) # a small addendum, converted float64 into float32

    obj_points = []
    img_points = []
    flag, frame = cap.read()
    h, w = frame.shape[:2]

    count = 0
    while(count<30): # we are using 30 images to calculate the camera matrix. This will yeild higher accuracy
        flag, frame = cap.read()
        found, corners = cv2.findChessboardCorners(frame, pattern_size)

        if found:
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            cv2.cornerSubPix(gray, corners, (5, 5), (-1, -1), criteria) # this yeilds corners with subpixel accuracy
            cv2.drawChessboardCorners(frame, pattern_size, corners, found)

            img_points.append(corners.reshape(-1, 2)) # for each of the 30 images, we append the list of image points
            obj_points.append(pattern_points) # likewise we append the object points for the 30 images
            count=count+1

        cv2.imshow("calibration", frame)
        key = cv2.waitKey(20)
        if(key==27):
            break

    flag, intrinsic_matrix, dist_coefs, rvecs, tvecs = cv2.calibrateCamera(obj_points, img_points, (w, h)) # camera_matrix yeilds the intrinsics, dist_coeffs correct radial distortions.
    print "intrinsic matrix:\n", intrinsic_matrix
    print "distortion coefficients: ", dist_coefs.ravel()
    cv2.destroyAllWindows()
