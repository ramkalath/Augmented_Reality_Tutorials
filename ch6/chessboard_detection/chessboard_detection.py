# *****************************************************************************
# * Filename : chessboard_detection.py
# * Date : 31/May/2018
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : Implements chessboard detection
# * Detailed Description : type ./run.sh to run this code
# *****************************************************************************
import numpy as np
import cv2

criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# this describes the shape of the chessboard corners we are tracking - outer corners are discarded
cols = 9
rows = 6

if __name__ == "__main__":
    cap = cv2.VideoCapture(0)
    count = 0
    while(count < 1000):  # Here, 10 can be changed to whatever number you like to choose
        flag, frame = cap.read() # Capture frame-by-frame
        gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # Find the chess board corners
        flag, corners = cv2.findChessboardCorners(gray_frame, (cols, rows), None) # the variable "corners" contains the pixel information of the tracked chessboard corners
        if flag == True:
            print count, ") track successful"

        # painting corners on the frame
        cv2.drawChessboardCorners(frame, (cols,rows), corners, flag)
        count += 1

        # display
        cv2.imshow("tracked chessboard", frame)
        key = cv2.waitKey(10)
        if(key==27):
            break

    # release the used resources
    cap.release()
    cv2.destroyAllWindows()
